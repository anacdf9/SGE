#ifndef CLIENTE_H
#define CLIENTE_H

#define CLIENTE_NOME_LEN 100
#define CLIENTE_ENDERECO_LEN 150
#define CLIENTE_CPF_CNPJ_LEN 30
#define CLIENTE_TELEFONE_LEN 30
#define CLIENTE_EMAIL_LEN 80
#define CLIENTE_CONTATO_LEN 80

typedef struct {
    int id; // código único
    char nome[CLIENTE_NOME_LEN];
    char endereco[CLIENTE_ENDERECO_LEN];
    char cpf_cnpj[CLIENTE_CPF_CNPJ_LEN];
    char telefone[CLIENTE_TELEFONE_LEN];
    char email[CLIENTE_EMAIL_LEN];
    char contato[CLIENTE_CONTATO_LEN];
} Cliente;

#endif // CLIENTE_H
