#ifndef PRODUTORA_H
#define PRODUTORA_H

#define PROD_NOME_FANTASIA_LEN 120
#define PROD_RAZAO_SOCIAL_LEN 120
#define PROD_INSCRICAO_ESTADUAL_LEN 40
#define PROD_CNPJ_LEN 30
#define PROD_ENDERECO_LEN 150
#define PROD_TELEFONE_LEN 30
#define PROD_EMAIL_LEN 80
#define PROD_RESPONSAVEL_LEN 80
#define PROD_TEL_RESP_LEN 30

/* Cadastro único da Produtora */
typedef struct {
    char nome_fantasia[PROD_NOME_FANTASIA_LEN];
    char razao_social[PROD_RAZAO_SOCIAL_LEN];
    char inscricao_estadual[PROD_INSCRICAO_ESTADUAL_LEN];
    char cnpj[PROD_CNPJ_LEN];
    char endereco[PROD_ENDERECO_LEN];
    char telefone[PROD_TELEFONE_LEN];
    char email[PROD_EMAIL_LEN];
    char responsavel[PROD_RESPONSAVEL_LEN];
    char telefone_responsavel[PROD_TEL_RESP_LEN];
    double margem_lucro_padrao; /* em % ou fator, aqui usamos % (0-100) */
} Produtora;

#endif