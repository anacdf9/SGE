#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pers_texto.h"

static const char *ARQ_CLIENTES = "clientes.txt";
static const char *ARQ_RECURSOS = "recursos.txt";
static const char *ARQ_EQUIPES  = "equipes.txt";
static const char *ARQ_FORNEC   = "fornecedores.txt";
static const char *ARQ_OPER     = "operadores.txt";
static const char *ARQ_PROD     = "produtora.txt";

/*
// ESSA FUNÇÃO NÃO ESTAVA SENDO USADA, TAVA DANDO WARNING. COMENTEI ELA.
static void csv_escape(const char *in, char *out, int outsize) {
    int k = 0;
    for (int i = 0; in[i] != '\0' && k + 1 < outsize; i++) {
        char ch = in[i];
        if (ch == ';') {
            if (k + 1 < outsize) out[k++] = ',';
        } else {
            out[k++] = ch;
        }
    }
    out[k] = '\0';
}
*/

void pers_texto_inicializar(void) {
    /* nada especial (arquivo criado quando salvarmos) */
}

void pers_texto_finalizar(void) {
    /* nada especial */
}

int pers_texto_salvar_cliente(Cliente c) {
    /* estratégia simples: ler todos, substituir por id se existir, senão append */
    
    // Cliente *arr = NULL; // <-- VARIÁVEL NÃO USADA, TIREI O WARNING
    // int n = pers_texto_carregar_clientes(NULL, 0); // <-- VARIÁVEL NÃO USADA, TIREI O WARNING
    
    /* Implementação prática: reescrever todo o arquivo (safe): carregar todos para memória dinâmica */
    FILE *f = fopen(ARQ_CLIENTES, "r");
    Cliente temp;
    Cliente *lista = NULL;
    int total = 0;
    if (f) {
        while (fscanf(f, "%d;%99[^;];%149[^;];%29[^;];%29[^;];%79[^;];%79[^\n]\n",
                      &temp.id, temp.nome, temp.endereco, temp.cpf_cnpj, temp.telefone, temp.email, temp.contato) == 7) {
            lista = realloc(lista, sizeof(Cliente) * (total + 1));
            if (!lista) { fclose(f); return 0; }
            lista[total++] = temp;
        }
        fclose(f);
    }
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id == c.id) {
            lista[i] = c;
            found = 1;
            break;
        }
    }
    if (!found) {
        lista = realloc(lista, sizeof(Cliente) * (total + 1));
        if (!lista) return 0;
        lista[total++] = c;
    }
    /* reescreve */
    f = fopen(ARQ_CLIENTES, "w");
    if (!f) { free(lista); return 0; }
    for (int i = 0; i < total; i++) {
        /* escreve CSV separado por ; com limites */
        fprintf(f, "%d;%s;%s;%s;%s;%s;%s\n",
                lista[i].id,
                lista[i].nome,
                lista[i].endereco,
                lista[i].cpf_cnpj,
                lista[i].telefone,
                lista[i].email,
                lista[i].contato);
    }
    fclose(f);
    free(lista);
    return 1;
}

int pers_texto_carregar_clientes(Cliente *lista, int max) {
    FILE *f = fopen(ARQ_CLIENTES, "r");
    if (!f) return 0;
    int total = 0;
    Cliente temp;
    while (fscanf(f, "%d;%99[^;];%149[^;];%29[^;];%29[^;];%79[^;];%79[^\n]\n",
                  &temp.id, temp.nome, temp.endereco, temp.cpf_cnpj, temp.telefone, temp.email, temp.contato) == 7) {
        if (lista && total < max) lista[total] = temp;
        total++;
    }
    fclose(f);
    /* se lista==NULL, retornamos o total (mas a chamada feita aqui sempre passa buffer) */
    if (lista) return (total < max) ? total : max;
    return total;
}

int pers_texto_remover_cliente(int id) {
    FILE *f = fopen(ARQ_CLIENTES, "r");
    if (!f) return 0;
    Cliente *lista = NULL;
    int total = 0;
    Cliente temp;
    while (fscanf(f, "%d;%99[^;];%149[^;];%29[^;];%29[^;];%79[^;];%79[^\n]\n",
                  &temp.id, temp.nome, temp.endereco, temp.cpf_cnpj, temp.telefone, temp.email, temp.contato) == 7) {
        lista = realloc(lista, sizeof(Cliente) * (total + 1));
        if (!lista) { fclose(f); return 0; }
        lista[total++] = temp;
    }
    fclose(f);
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id == id) { /* remove: shift left */
            for (int j = i; j < total - 1; j++) lista[j] = lista[j + 1];
            total--;
            found = 1;
            break;
        }
    }
    if (!found) { free(lista); return 0; }
    f = fopen(ARQ_CLIENTES, "w");
    if (!f) { free(lista); return 0; }
    for (int i = 0; i < total; i++) {
        fprintf(f, "%d;%s;%s;%s;%s;%s;%s\n",
                lista[i].id,
                lista[i].nome,
                lista[i].endereco,
                lista[i].cpf_cnpj,
                lista[i].telefone,
                lista[i].email,
                lista[i].contato);
    }
    fclose(f);
    free(lista);
    return 1;
}

/* ===== Recurso ===== */
int pers_texto_salvar_recurso(Recurso r) {
    FILE *f = fopen(ARQ_RECURSOS, "r");
    Recurso *lista = NULL, temp; int total = 0;
    if (f) {
        while (fscanf(f, "%d;%119[^;];%59[^;];%d;%lf;%lf\n",
                      &temp.id, temp.descricao, temp.categoria, &temp.quantidade,
                      &temp.preco_custo, &temp.valor_locacao) == 6) {
            lista = realloc(lista, sizeof(Recurso) * (total + 1)); if (!lista){fclose(f);return 0;}
            lista[total++] = temp;
        }
        fclose(f);
    }
    int found = 0; for (int i=0;i<total;i++){ if (lista[i].id==r.id){lista[i]=r;found=1;break;} }
    if (!found){ lista = realloc(lista, sizeof(Recurso)*(total+1)); if(!lista) return 0; lista[total++]=r; }
    f = fopen(ARQ_RECURSOS, "w"); if(!f){ free(lista); return 0; }
    for (int i=0;i<total;i++) fprintf(f, "%d;%s;%s;%d;%.2f;%.2f\n",
        lista[i].id, lista[i].descricao, lista[i].categoria, lista[i].quantidade,
        lista[i].preco_custo, lista[i].valor_locacao);
    fclose(f); free(lista); return 1;
}

int pers_texto_carregar_recursos(Recurso *lista, int max) {
    FILE *f = fopen(ARQ_RECURSOS, "r"); if(!f) return 0; int total=0; Recurso temp;
    while (fscanf(f, "%d;%119[^;];%59[^;];%d;%lf;%lf\n",
                  &temp.id, temp.descricao, temp.categoria, &temp.quantidade,
                  &temp.preco_custo, &temp.valor_locacao) == 6) {
        if (lista && total<max) lista[total]=temp; total++;
    }
    fclose(f); if (lista) return (total<max)?total:max; return total;
}

int pers_texto_remover_recurso(int id) {
    FILE *f = fopen(ARQ_RECURSOS, "r"); if(!f) return 0; Recurso *lista=NULL, temp; int total=0;
    while (fscanf(f, "%d;%119[^;];%59[^;];%d;%lf;%lf\n",
                  &temp.id, temp.descricao, temp.categoria, &temp.quantidade,
                  &temp.preco_custo, &temp.valor_locacao) == 6) {
        lista = realloc(lista, sizeof(Recurso)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;
    }
    fclose(f);
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } }
    if(!found){ free(lista); return 0; }
    f=fopen(ARQ_RECURSOS, "w"); if(!f){ free(lista); return 0; }
    for(int i=0;i<total;i++) fprintf(f, "%d;%s;%s;%d;%.2f;%.2f\n",
        lista[i].id, lista[i].descricao, lista[i].categoria, lista[i].quantidade,
        lista[i].preco_custo, lista[i].valor_locacao);
    fclose(f); free(lista); return 1;
}

/* ===== Equipe ===== */
int pers_texto_salvar_equipe(Equipe e) {
    FILE *f=fopen(ARQ_EQUIPES, "r"); Equipe *lista=NULL, temp; int total=0;
    if (f){ while(fscanf(f, "%d;%99[^;];%19[^;];%59[^;];%lf\n",
                         &temp.id, temp.nome, temp.cpf, temp.funcao, &temp.valor_hora)==5){
            lista=realloc(lista,sizeof(Equipe)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp; }
        fclose(f); }
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==e.id){lista[i]=e;found=1;break;} }
    if(!found){ lista=realloc(lista,sizeof(Equipe)*(total+1)); if(!lista) return 0; lista[total++]=e; }
    f=fopen(ARQ_EQUIPES, "w"); if(!f){ free(lista); return 0; }
    for(int i=0;i<total;i++) fprintf(f, "%d;%s;%s;%s;%.2f\n",
        lista[i].id, lista[i].nome, lista[i].cpf, lista[i].funcao, lista[i].valor_hora);
    fclose(f); free(lista); return 1;
}

int pers_texto_carregar_equipes(Equipe *lista, int max) {
    FILE *f=fopen(ARQ_EQUIPES, "r"); if(!f) return 0; int total=0; Equipe temp;
    while(fscanf(f, "%d;%99[^;];%19[^;];%59[^;];%lf\n",
                 &temp.id, temp.nome, temp.cpf, temp.funcao, &temp.valor_hora)==5){
        if(lista && total<max) lista[total]=temp; total++; }
    fclose(f); if(lista) return (total<max)?total:max; return total;
}

int pers_texto_remover_equipe(int id) {
    FILE *f=fopen(ARQ_EQUIPES, "r"); if(!f) return 0; Equipe *lista=NULL, temp; int total=0;
    while(fscanf(f, "%d;%99[^;];%19[^;];%59[^;];%lf\n",
                 &temp.id, temp.nome, temp.cpf, temp.funcao, &temp.valor_hora)==5){
        lista=realloc(lista,sizeof(Equipe)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp; }
    fclose(f);
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } }
    if(!found){ free(lista); return 0; }
    f=fopen(ARQ_EQUIPES, "w"); if(!f){ free(lista); return 0; }
    for(int i=0;i<total;i++) fprintf(f, "%d;%s;%s;%s;%.2f\n",
        lista[i].id, lista[i].nome, lista[i].cpf, lista[i].funcao, lista[i].valor_hora);
    fclose(f); free(lista); return 1;
}

/* ===== Fornecedor ===== */
int pers_texto_salvar_fornecedor(Fornecedor fz) {
    FILE *f=fopen(ARQ_FORNEC, "r"); Fornecedor *lista=NULL, temp; int total=0;
    if(f){ while(fscanf(f, "%d;%119[^;];%119[^;];%29[^;];%149[^;];%29[^;];%79[^\n]\n",
                        &temp.id, temp.nome_fantasia, temp.razao_social, temp.cnpj_cpf,
                        temp.endereco, temp.telefone, temp.tipo_servico)==7){
            lista=realloc(lista,sizeof(Fornecedor)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp; }
        fclose(f);} 
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==fz.id){lista[i]=fz;found=1;break;} }
    if(!found){ lista=realloc(lista,sizeof(Fornecedor)*(total+1)); if(!lista) return 0; lista[total++]=fz; }
    f=fopen(ARQ_FORNEC, "w"); if(!f){ free(lista); return 0; }
    for(int i=0;i<total;i++) fprintf(f, "%d;%s;%s;%s;%s;%s;%s\n",
        lista[i].id, lista[i].nome_fantasia, lista[i].razao_social, lista[i].cnpj_cpf,
        lista[i].endereco, lista[i].telefone, lista[i].tipo_servico);
    fclose(f); free(lista); return 1;
}

int pers_texto_carregar_fornecedores(Fornecedor *lista, int max) {
    FILE *f=fopen(ARQ_FORNEC, "r"); if(!f) return 0; int total=0; Fornecedor temp;
    while(fscanf(f, "%d;%119[^;];%119[^;];%29[^;];%149[^;];%29[^;];%79[^\n]\n",
                 &temp.id, temp.nome_fantasia, temp.razao_social, temp.cnpj_cpf,
                 temp.endereco, temp.telefone, temp.tipo_servico)==7){ if(lista && total<max) lista[total]=temp; total++; }
    fclose(f); if(lista) return (total<max)?total:max; return total;
}

int pers_texto_remover_fornecedor(int id) {
    FILE *f=fopen(ARQ_FORNEC, "r"); if(!f) return 0; Fornecedor *lista=NULL, temp; int total=0;
    while(fscanf(f, "%d;%119[^;];%119[^;];%29[^;];%149[^;];%29[^;];%79[^\n]\n",
                 &temp.id, temp.nome_fantasia, temp.razao_social, temp.cnpj_cpf,
                 temp.endereco, temp.telefone, temp.tipo_servico)==7){ lista=realloc(lista,sizeof(Fornecedor)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp; }
    fclose(f);
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } }
    if(!found){ free(lista); return 0; }
    f=fopen(ARQ_FORNEC, "w"); if(!f){ free(lista); return 0; }
    for(int i=0;i<total;i++) fprintf(f, "%d;%s;%s;%s;%s;%s;%s\n",
        lista[i].id, lista[i].nome_fantasia, lista[i].razao_social, lista[i].cnpj_cpf,
        lista[i].endereco, lista[i].telefone, lista[i].tipo_servico);
    fclose(f); free(lista); return 1;
}

/* ===== Operador ===== */
int pers_texto_salvar_operador(Operador o) {
    FILE *f=fopen(ARQ_OPER, "r"); Operador *lista=NULL, temp; int total=0;
    if(f){ while(fscanf(f, "%d;%99[^;];%39[^;];%39[^\n]\n",
                        &temp.id, temp.nome, temp.usuario, temp.senha)==4){ lista=realloc(lista,sizeof(Operador)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp; }
        fclose(f);} 
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==o.id){lista[i]=o;found=1;break;} }
    if(!found){ lista=realloc(lista,sizeof(Operador)*(total+1)); if(!lista) return 0; lista[total++]=o; }
    f=fopen(ARQ_OPER, "w"); if(!f){ free(lista); return 0; }
    for(int i=0;i<total;i++) fprintf(f, "%d;%s;%s;%s\n", lista[i].id, lista[i].nome, lista[i].usuario, lista[i].senha);
    fclose(f); free(lista); return 1;
}

int pers_texto_carregar_operadores(Operador *lista, int max) {
    FILE *f=fopen(ARQ_OPER, "r"); if(!f) return 0; int total=0; Operador temp;
    while(fscanf(f, "%d;%99[^;];%39[^;];%39[^\n]\n", &temp.id, temp.nome, temp.usuario, temp.senha)==4){ if(lista && total<max) lista[total]=temp; total++; }
    fclose(f); if(lista) return (total<max)?total:max; return total;
}

int pers_texto_remover_operador(int id) {
    FILE *f=fopen(ARQ_OPER, "r"); if(!f) return 0; Operador *lista=NULL, temp; int total=0;
    while(fscanf(f, "%d;%99[^;];%39[^;];%39[^\n]\n", &temp.id, temp.nome, temp.usuario, temp.senha)==4){ lista=realloc(lista,sizeof(Operador)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp; }
    fclose(f);
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } }
    if(!found){ free(lista); return 0; }
    f=fopen(ARQ_OPER, "w"); if(!f){ free(lista); return 0; }
    for(int i=0;i<total;i++) fprintf(f, "%d;%s;%s;%s\n", lista[i].id, lista[i].nome, lista[i].usuario, lista[i].senha);
    fclose(f); free(lista); return 1;
}

/* ===== Produtora (única) ===== */
int pers_texto_salvar_produtora(Produtora p) {
    /* grava em uma única linha CSV, igual aos outros cadastros */
    FILE *f = fopen(ARQ_PROD, "w");
    if (!f) return 0;
    fprintf(f, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%.2f\n",
            p.nome_fantasia,
            p.razao_social,
            p.inscricao_estadual,
            p.cnpj,
            p.endereco,
            p.telefone,
            p.email,
            p.responsavel,
            p.telefone_responsavel,
            p.margem_lucro_padrao);
    fclose(f);
    return 1;
}

int pers_texto_obter_produtora(Produtora *out) {
    if (!out) return 0;
    FILE *f = fopen(ARQ_PROD, "r");
    if (!f) return 0;
    Produtora p;
    int ok = fscanf(f,
                    "%119[^;];%119[^;];%39[^;];%29[^;];%149[^;];%29[^;];%79[^;];%79[^;];%29[^;];%lf\n",
                    p.nome_fantasia,
                    p.razao_social,
                    p.inscricao_estadual,
                    p.cnpj,
                    p.endereco,
                    p.telefone,
                    p.email,
                    p.responsavel,
                    p.telefone_responsavel,
                    &p.margem_lucro_padrao) == 10;
    fclose(f);
    if (!ok) return 0;
    *out = p;
    return 1;
}

int pers_texto_remover_produtora(void) {
    /* remove o arquivo de produtora (cadastro único) */
    int rc = remove(ARQ_PROD);
    return rc == 0 ? 1 : 0;
}