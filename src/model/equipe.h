#ifndef EQUIPE_H
#define EQUIPE_H

#define EQUIPE_NOME_LEN 100
#define EQUIPE_CPF_LEN 20
#define EQUIPE_FUNCAO_LEN 60

typedef struct {
    int id;            /* Código (único) */
    char nome[EQUIPE_NOME_LEN];
    char cpf[EQUIPE_CPF_LEN];
    char funcao[EQUIPE_FUNCAO_LEN];
    double valor_hora; /* diária/hora */
} Equipe;

#endif