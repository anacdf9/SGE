#ifndef FORNECEDOR_H
#define FORNECEDOR_H

#define FORN_NOME_FANTASIA_LEN 120
#define FORN_RAZAO_SOCIAL_LEN 120
#define FORN_DOC_LEN 30
#define FORN_ENDERECO_LEN 150
#define FORN_TELEFONE_LEN 30
#define FORN_SERVICO_LEN 80

typedef struct {
    int id; /* Código (único) */
    char nome_fantasia[FORN_NOME_FANTASIA_LEN];
    char razao_social[FORN_RAZAO_SOCIAL_LEN];
    char cnpj_cpf[FORN_DOC_LEN];
    char endereco[FORN_ENDERECO_LEN];
    char telefone[FORN_TELEFONE_LEN];
    char tipo_servico[FORN_SERVICO_LEN];
} Fornecedor;

#endif