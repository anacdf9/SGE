CC = gcc
TARGET = SGE.exe

# Caminhos do IUP
IUP_INCLUDE_PATH = D:/Codes/resposVS/SGE/lib/iup/include
IUP_LIB_PATH = D:/Codes/resposVS/SGE/lib/iup

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
	src/view/produtora_view.c

OBJS = $(SRCS:.c=.o)

# === Regras ===
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo.
	@echo --- Linkando programa final: $(TARGET) ---
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo --- Compilação concluída com sucesso! ---

%.o: %.c
	@echo.
	@echo --- Compilando: $< ---
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo.
	@echo --- Limpando arquivos compilados ---
	@if exist $(TARGET) del /f /q $(TARGET)
	@for %%f in ($(OBJS)) do if exist "%%f" del /f /q "%%f"
	@echo --- Limpeza concluída ---
