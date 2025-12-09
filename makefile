CC ?= cc
# Seleção automática de compilador disponível no PATH
# Tenta: x86_64-w64-mingw32-gcc (MinGW 64), i686-w64-mingw32-gcc (MinGW 32), gcc (nativo)
HAVE_CC := $(shell command -v $(CC) 2>/dev/null)
ifeq ($(HAVE_CC),)
	HAVE_CC := $(shell command -v x86_64-w64-mingw32-gcc 2>/dev/null)
	ifneq ($(HAVE_CC),)
		CC := x86_64-w64-mingw32-gcc
	else
		HAVE_CC := $(shell command -v i686-w64-mingw32-gcc 2>/dev/null)
		ifneq ($(HAVE_CC),)
			CC := i686-w64-mingw32-gcc
		else
			HAVE_CC := $(shell command -v gcc 2>/dev/null)
			ifneq ($(HAVE_CC),)
				CC := gcc
			else
				$(error Compiler not found in PATH)
			endif
		endif
	endif
endif

# Determina o executável (MinGW = .exe, Linux = sem extensão)
ifeq ($(CC),x86_64-w64-mingw32-gcc)
	TARGET = SGE.exe
	UNAME_S := Windows
else ifeq ($(CC),i686-w64-mingw32-gcc)
	TARGET = SGE.exe
	UNAME_S := Windows
else
	TARGET = SGE
	UNAME_S := $(shell uname -s)
endif

# === Detecção de arquitetura e diretórios ===
# ARCH pode ser: auto | 32 | 64
ARCH ?= auto

# Já garantimos a existência do compilador acima

# Detecta triplet do compilador (ex.: i686-w64-mingw32, x86_64-w64-mingw32)
CC_MACHINE := $(shell $(CC) -dumpmachine 2>/dev/null)
TRY_M32 := $(strip $(shell $(CC) -m32 -v 1>/dev/null 2>&1 && echo yes))

# Decide a arquitetura alvo
ifeq ($(ARCH),auto)
	ifneq (,$(findstring x86_64,$(CC_MACHINE)))
		TARGET_ARCH := 64
	else
		TARGET_ARCH := 32
	endif
else
	TARGET_ARCH := $(ARCH)
endif

# Diretórios IUP/CD conforme arquitetura alvo (permitir override externo via linha de comando)
IUP_DIR ?= $(CURDIR)/lib/iup$(TARGET_ARCH)
CD_DIR  ?= $(CURDIR)/lib/cd$(TARGET_ARCH)

IUP_INCLUDE_PATH = $(IUP_DIR)/include
IUP_LIB_PATH     = $(IUP_DIR)
CD_LIB_PATH      = $(CD_DIR)

IUP32_INCLUDE := $(CURDIR)/lib/iup32/include
IUP64_INCLUDE := $(CURDIR)/lib/iup64/include

# Fallback: se alvo 64-bit sem SDK 64, mas temos SDK 32, tentar compilar 32-bit
ifeq ($(TARGET_ARCH),64)
	ifeq ($(wildcard $(IUP_INCLUDE_PATH)/iup.h),)
		ifneq ($(wildcard $(IUP32_INCLUDE)/iup.h),)
			HAVE_I686 := $(shell command -v i686-w64-mingw32-gcc 2>/dev/null)
			ifneq ($(HAVE_I686),)
				$(info 64-bit IUP SDK not found; falling back to 32-bit toolchain)
				CC := i686-w64-mingw32-gcc
				TARGET_ARCH := 32
				IUP_DIR := $(CURDIR)/lib/iup32
				CD_DIR  := $(CURDIR)/lib/cd32
				IUP_INCLUDE_PATH := $(IUP_DIR)/include
				IUP_LIB_PATH     := $(IUP_DIR)
				CD_LIB_PATH      := $(CD_DIR)
			else ifeq ($(TRY_M32),yes)
				$(info 64-bit IUP SDK not found; using $(CC) with -m32 to link against 32-bit SDK)
				TARGET_ARCH := 32
				IUP_DIR := $(CURDIR)/lib/iup32
				CD_DIR  := $(CURDIR)/lib/cd32
				IUP_INCLUDE_PATH := $(IUP_DIR)/include
				IUP_LIB_PATH     := $(IUP_DIR)
				CD_LIB_PATH      := $(CD_DIR)
				CFLAGS += -m32
				LDFLAGS += -m32
			endif
		endif
	endif
endif

ifeq ($(wildcard $(IUP_INCLUDE_PATH)/iup.h),)
$(error IUP headers not found in $(IUP_INCLUDE_PATH). Expected $(TARGET_ARCH)-bit SDK. Copy the right IUP SDK to $(IUP_DIR) or pass IUP_DIR="<path>")
endif

$(info CC: $(CC))
$(info CC_MACHINE: $(CC_MACHINE))
$(info TARGET_ARCH: $(TARGET_ARCH))
$(info IUP_DIR: $(IUP_DIR))
$(info CD_DIR: $(CD_DIR))

# Bibliotecas
LIBS_IUP = -liup -liupcontrols

# Flags condicionais por sistema
ifeq ($(UNAME_S),Windows)
	LIBS_WIN = -lgdi32 -lcomctl32 -luser32 -lole32 -mwindows
else
	LIBS_WIN =
endif

# Flags
CFLAGS = -Wall -g -std=c99 -I$(IUP_INCLUDE_PATH) -Isrc -Isrc/model -Isrc/controller -Isrc/view
LDFLAGS = -L$(IUP_LIB_PATH) $(LIBS_IUP) $(LIBS_WIN)

# Fontes
SRCS = \
	main.c \
	src/controller/cliente_controller.c \
	src/controller/equipe_controller.c \
	src/controller/recurso_controller.c \
	src/controller/fornecedor_controller.c \
	src/controller/operador_controller.c \
	src/controller/produtora_controller.c \
	src/controller/evento_controller.c \
	src/controller/transacoes_controller.c \
	src/controller/feedback_controller.c \
	src/controller/trade_controller.c \
	src/model/cliente.c \
	src/model/pers.c \
	src/model/pers_memoria.c \
	src/model/pers_texto.c \
	src/model/pers_binario.c \
	src/model/evento.c \
	src/view/cliente_view.c \
	src/view/equipe_view.c \
	src/view/recurso_view.c \
	src/view/fornecedor_view.c \
	src/view/operador_view.c \
	src/view/evento_view.c \
	src/view/transacoes_view.c \
	src/view/produtora_view.c \
	src/view/feedback_view.c \
	src/view/trade_view.c \
	src/view/ui_common.c \
	src/view/validation.c

OBJS = $(SRCS:.c=.o)

# === Regras ===
.PHONY: all clean win32 win64 copy-dlls run
all: $(TARGET)

win32:
	@echo.
	@echo === Build 32-bit (MSYS2 MinGW32) ===
	$(MAKE) clean ARCH=32 CC=i686-w64-mingw32-gcc IUP_DIR=$(CURDIR)/lib/iup32 CD_DIR=$(CURDIR)/lib/cd32
	$(MAKE) all ARCH=32 CC=i686-w64-mingw32-gcc IUP_DIR=$(CURDIR)/lib/iup32 CD_DIR=$(CURDIR)/lib/cd32

win64:
	@echo.
	@echo === Build 64-bit (MSYS2 MinGW64) ===
	$(MAKE) clean ARCH=64 CC=x86_64-w64-mingw32-gcc IUP_DIR=$(CURDIR)/lib/iup64 CD_DIR=$(CURDIR)/lib/cd64
	$(MAKE) all ARCH=64 CC=x86_64-w64-mingw32-gcc IUP_DIR=$(CURDIR)/lib/iup64 CD_DIR=$(CURDIR)/lib/cd64

# DLLs de runtime necessários (copiados se existirem)
IUP_RUNTIME_DLLS = iup.dll iupcontrols.dll iupcd.dll zlib1.dll freetype6.dll ftgl.dll
CD_RUNTIME_DLLS = cd.dll

$(TARGET): $(OBJS)
	@echo ""
	@echo "--- Linkando programa final: $(TARGET) ---"
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "--- Compilação concluída com sucesso! ---"
	@echo ""
ifeq ($(UNAME_S),Windows)
	@echo "--- Copiando DLLs de runtime (IUP + CD) ---"
	@for DLL in $(IUP_RUNTIME_DLLS); do \
		if [ -f "$(IUP_LIB_PATH)/$$DLL" ]; then cp "$(IUP_LIB_PATH)/$$DLL" . ; fi ; \
	done
	@for DLL in $(CD_RUNTIME_DLLS); do \
		if [ -f "$(CD_LIB_PATH)/$$DLL" ]; then cp "$(CD_LIB_PATH)/$$DLL" . ; fi ; \
	done
endif

copy-dlls:
ifeq ($(UNAME_S),Windows)
	@echo "--- Atualizando DLLs de runtime (se existirem) ---"
	@for DLL in $(IUP_RUNTIME_DLLS); do \
		if [ -f "$(IUP_LIB_PATH)/$$DLL" ]; then cp "$(IUP_LIB_PATH)/$$DLL" . ; fi ; \
	done
	@for DLL in $(CD_RUNTIME_DLLS); do \
		if [ -f "$(CD_LIB_PATH)/$$DLL" ]; then cp "$(CD_LIB_PATH)/$$DLL" . ; fi ; \
	done
endif

run: $(TARGET) copy-dlls
	@echo "--- Executando programa ---"
ifeq ($(UNAME_S),Windows)
	@PATH="$(CD_LIB_PATH):$(IUP_LIB_PATH):$$PATH" ./$(TARGET)
else
	@LD_LIBRARY_PATH="$(CD_LIB_PATH):$(IUP_LIB_PATH):$$LD_LIBRARY_PATH" ./$(TARGET)
endif

%.o: %.c
	@echo ""
	@echo "--- Compilando: $< ---"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo ""
	@echo "--- Limpando arquivos compilados ---"
	@rm -f $(TARGET)
	@find . -name "*.o" -delete
	@echo "--- Limpeza concluída ---"
