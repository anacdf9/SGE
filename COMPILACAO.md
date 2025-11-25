# Guia de Compilação

Como compilar o SGE no Windows com MinGW

---

## Requisitos

### MinGW (Compiler)
- Download: https://www.mingw-w64.org/
- Versão: GCC 8.1.0 ou superior
- Adicionar `bin/` ao PATH do Windows

### Biblioteca IUP
- Já incluída no projeto (pasta `lib/`)
- Versão 64 bits (iup64, cd64)
- Não precisa instalar nada

---

## Compilar o Projeto

### Comandos Básicos

```bash
# Compilar tudo
mingw32-make

# Limpar e compilar
mingw32-make clean
mingw32-make

# Apenas limpar
mingw32-make clean
```

### O que o Makefile faz

1. Compila todos os `.c` em objetos `.o`
2. Linka com as bibliotecas IUP
3. Gera o executável `SGE.exe`

---

## Executar

```bash
./SGE.exe
```

ou

```bash
.\SGE.exe
```

---

## Estrutura do Makefile

```makefile
CC = gcc                    # Compilador
CFLAGS = -Wall -O2         # Flags de compilação
LIBS = -liup -lcomctl32    # Bibliotecas necessárias

# Compila cada .c em .o
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Linka tudo
SGE.exe: $(OBJS)
    $(CC) $(OBJS) -o $@ $(LIBS)
```

---

## Problemas Comuns

### Erro: "mingw32-make não reconhecido"

**Causa:** MinGW não está no PATH

**Solução:**
1. Abra Painel de Controle
2. Sistema > Configurações Avançadas
3. Variáveis de Ambiente
4. Edite PATH
5. Adicione: `C:\MinGW\bin` (ou onde instalou)

### Erro: "iup.h: No such file"

**Causa:** Compilador não achou a biblioteca IUP

**Solução:**
Verifique se as pastas existem:
```
lib/iup64/include/
lib/cd64/include/
```

Makefile deve ter:
```makefile
-Ilib/iup64/include
-Ilib/cd64/include
```

### Erro: "undefined reference to IupOpen"

**Causa:** Faltou linkar as bibliotecas

**Solução:**
Makefile deve ter:
```makefile
-Llib/iup64/Lua54
-liup -lcomctl32 -lole32 -lgdi32 -lcomdlg32
```

### Erro: "multiple definition of cliente_atualizar_cb"

**Causa:** Função definida 2 vezes

**Solução:**
Já corrigido. Uma das funções foi renomeada para `cliente_novo_cb`

### Warning: "format '%d' expects int"

**Causa:** Tipo errado no printf/scanf

**Solução:**
```c
// Errado
int id;
scanf("%d", id);  // Faltou o &

// Certo
int id;
scanf("%d", &id);
```

---

## Dicas de Compilação

### Compilar Apenas um Arquivo

```bash
gcc -c src/model/cliente.c -o cliente.o -Ilib/iup64/include
```

### Ver Todos os Warnings

```bash
mingw32-make CFLAGS="-Wall -Wextra -Wpedantic"
```

### Compilar em Modo Debug

```bash
mingw32-make CFLAGS="-Wall -g"
```

O `-g` adiciona informações de debug para usar com GDB

### Usar o GDB (Debugger)

```bash
gdb ./SGE.exe

# Dentro do GDB:
(gdb) break main        # Breakpoint na main
(gdb) run              # Executar
(gdb) next             # Próxima linha
(gdb) print variavel   # Ver valor de variável
(gdb) quit             # Sair
```

---

## Estrutura de Arquivos de Build

```
SGE/
├── SGE.exe                 # Executável final
├── *.o                     # Arquivos objeto
└── src/
    ├── model/*.o
    ├── controller/*.o
    └── view/*.o
```

---

## Recompilar Após Mudanças

### Mudou apenas um arquivo .c
```bash
mingw32-make
```
Make é inteligente e só recompila o necessário

### Mudou um arquivo .h
```bash
mingw32-make clean
mingw32-make
```
Melhor recompilar tudo porque o .h pode afetar vários arquivos

### Adicionou novo arquivo
```bash
# Adicione no Makefile:
OBJS = ... novo_arquivo.o

# Compile
mingw32-make clean
mingw32-make
```

---

## Otimização

### Compilar Mais Rápido
```makefile
CFLAGS = -O0  # Sem otimização (mais rápido)
```

### Executável Menor e Mais Rápido
```makefile
CFLAGS = -O2 -s  # Otimizado e stripped
```

### Executável Otimizado ao Máximo
```makefile
CFLAGS = -O3 -march=native -flto
```

---

## Arquivos Gerados

Após compilar com sucesso:
- `SGE.exe` - Executável principal (pode distribuir)
- `*.o` - Arquivos objeto (pode deletar)

Para limpar:
```bash
mingw32-make clean
```

---

## Verificar Versões

```bash
# GCC
gcc --version

# Make
mingw32-make --version
```

---

## Documentação Adicional

- Makefile oficial GNU: https://www.gnu.org/software/make/
- GCC Manual: https://gcc.gnu.org/onlinedocs/
- MinGW: https://www.mingw-w64.org/
- IUP: https://www.tecgraf.puc-rio.br/iup/
