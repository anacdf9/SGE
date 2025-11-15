CC ?= gcc
# Garante que CC aponte para um compilador existente (prioriza 32 bits)
CC_OK := $(shell where $(CC) >NUL 2>NUL && echo yes)
ifeq ($(CC_OK),)
	GCC_OK := $(shell where gcc >NUL 2>NUL && echo yes)
	ifneq ($(GCC_OK),)
		CC := gcc
	else
		I686_OK := $(shell where i686-w64-mingw32-gcc >NUL 2>NUL && echo yes)
		ifneq ($(I686_OK),)
			CC := i686-w64-mingw32-gcc
		endif
	endif
endif
TARGET = SGE.exe

# Caminhos do IUP
# Diretórios do IUP (somente 32 bits)
IUP_DIR32 ?= $(CURDIR)/lib/iup32
IUP_DIR ?= $(IUP_DIR32)

# Detecção simples do toolchain (informativo)
MACHINE := $(shell $(CC) -dumpmachine 2>NUL)
ARCH := 32

# Ajusta IUP_DIR automaticamente se o include padrão não existir
ifeq ($(wildcard $(IUP_DIR)/include/iup.h),)
  ifneq ($(wildcard $(IUP_DIR32)/include/iup.h),)
	IUP_DIR := $(IUP_DIR32)
  endif
endif

IUP_INCLUDE_PATH = $(IUP_DIR)/include
IUP_LIB_PATH = $(IUP_DIR)

# Diretório do Canvas Draw (CD) 32-bit
CD_DIR ?= $(CURDIR)/lib/cd32
CD_LIB_PATH = $(CD_DIR)

ifeq ($(wildcard $(IUP_INCLUDE_PATH)/iup.h),)
$(warning IUP headers not found in $(IUP_INCLUDE_PATH). If you installed IUP elsewhere, run:)
$(warning     make IUP_DIR="C:/path/to/iup")
endif
$(info CC: $(CC))
$(info GCC target: $(MACHINE) | ARCH: $(ARCH)-bit)
$(info IUP_DIR: $(IUP_DIR))
$(info CD_DIR: $(CD_DIR))

## Somente 32 bits suportado neste Makefile

MINGW32_CC ?= i686-w64-mingw32-gcc

# Bibliotecas
LIBS_WIN = -lgdi32 -lcomctl32 -luser32 -lole32 -mwindows
LIBS_IUP = -liup -liupcontrols


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
    src/model/cliente.c \
    src/model/pers.c \
    src/model/pers_memoria.c \
    src/model/pers_texto.c \
    src/model/pers_binario.c \
	src/view/cliente_view.c \
	src/view/equipe_view.c \
	src/view/recurso_view.c \
	src/view/fornecedor_view.c \
	src/view/operador_view.c \
	src/view/produtora_view.c \
	src/view/ui_common.c \
	src/view/validation.c

OBJS = $(SRCS:.c=.o)

# === Regras ===
.PHONY: all clean win32
all: $(TARGET)

win32:
	@echo.
	@echo === Build 32-bit (MSYS2 MinGW32) ===
	$(MAKE) clean
	$(MAKE) CC=$(MINGW32_CC) IUP_DIR=$(IUP_DIR32) all


# DLLs de runtime necessários (copiados se existirem)
IUP_RUNTIME_DLLS = iup.dll iupcontrols.dll iupcd.dll zlib1.dll freetype6.dll ftgl.dll
CD_RUNTIME_DLLS = cd.dll

$(TARGET): $(OBJS)
	@echo.
	@echo --- Linkando programa final: $(TARGET) ---
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo --- Compilacao concluida com sucesso! ---
	@echo.
	@echo --- Copiando DLLs de runtime (IUP + CD) ---
	@for %%D in ($(IUP_RUNTIME_DLLS)) do ( if exist "$(IUP_LIB_PATH)\%%D" copy /Y "$(IUP_LIB_PATH)\%%D" . >nul )
	@for %%D in ($(CD_RUNTIME_DLLS)) do ( if exist "$(CD_LIB_PATH)\%%D" copy /Y "$(CD_LIB_PATH)\%%D" . >nul )

.PHONY: copy-dlls
copy-dlls:
	@echo --- Atualizando DLLs de runtime (se existirem) ---
	@for %%D in ($(IUP_RUNTIME_DLLS)) do ( if exist "$(IUP_LIB_PATH)\%%D" copy /Y "$(IUP_LIB_PATH)\%%D" . >nul )
	@for %%D in ($(CD_RUNTIME_DLLS)) do ( if exist "$(CD_LIB_PATH)\%%D" copy /Y "$(CD_LIB_PATH)\%%D" . >nul )

.PHONY: run
run: $(TARGET) copy-dlls
	@echo --- Executando com PATH de IUP/CD preferenciais ---
	@set "PATH=$(CD_LIB_PATH);$(IUP_LIB_PATH);%PATH%" & .\$(TARGET)

%.o: %.c
	@echo.
	@echo --- Compilando: $< ---
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo.
	@echo --- Limpando arquivos compilados ---
	@-del /f /q "$(TARGET)" 2>nul
	@-del /f /q /s *.o 2>nul
	@echo --- Limpeza concluida ---
