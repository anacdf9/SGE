#ifndef OPERADOR_H
#define OPERADOR_H

#define OPERADOR_NOME_LEN 100
#define OPERADOR_USER_LEN 40
#define OPERADOR_SENHA_LEN 40

typedef struct {
    int id; /* Código (único) */
    char nome[OPERADOR_NOME_LEN];
    char usuario[OPERADOR_USER_LEN];
    char senha[OPERADOR_SENHA_LEN];
} Operador;

#endif