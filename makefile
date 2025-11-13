CC = gcc
TARGET = SGE.exe

# IUP
IUP_INCLUDE_PATH = D:/Codes/resposVS/SGE/lib/iup/include
IUP_LIB_PATH = D:/Codes/resposVS/SGE/lib/iup

LIBS_WIN = -lgdi32 -lcomctl32 -luser32 -lole32 -mwindows
LIBS_IUP = -liup -liupcontrols

CFLAGS = -Wall -g -std=c99 -I$(IUP_INCLUDE_PATH) -Isrc -Isrc/model -Isrc/controller -Isrc/view
LDFLAGS = -L$(IUP_LIB_PATH) $(LIBS_IUP) $(LIBS_WIN)

SRCS = \
    main.c \
    src/controller/cliente_controller.c \
    src/model/cliente.c \
    src/model/pers.c \
    src/model/pers_memoria.c \
    src/model/pers_texto.c \
    src/model/pers_binario.c \
    src/view/cliente_view.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "--- Linkando o programa final: $(TARGET) ---"
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "--- Compilacao concluida! ---"

%.o: %.c
	@echo "--- Compilando: $< ---"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "--- Limpando arquivos compilados ---"
	-del /F /Q $(OBJS) $(TARGET) 2>NUL || (exit 0)
