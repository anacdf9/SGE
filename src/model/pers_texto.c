#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pers_texto.h"
#include "evento_item.h"
#include "evento_equipe.h"
#include "evento_fornecedor.h"
#include "transacoes.h"

static const char *ARQ_CLIENTES = "clientes.txt";
static const char *ARQ_RECURSOS = "recursos.txt";
static const char *ARQ_EQUIPES  = "equipes.txt";
static const char *ARQ_FORNEC   = "fornecedores.txt";
static const char *ARQ_OPER     = "operadores.txt";
static const char *ARQ_PROD     = "produtora.txt";
static const char *ARQ_EVENTOS  = "eventos.txt";
static const char *ARQ_EVENTOS_ITENS = "eventos_itens.txt";
static const char *ARQ_EVENTOS_EQUIPES = "eventos_equipes.txt";
static const char *ARQ_EVENTOS_FORNECEDORES = "eventos_fornecedores.txt";
static const char *ARQ_CAIXA = "caixa.txt";
static const char *ARQ_CONTAS_RECEBER = "contas_receber.txt";
static const char *ARQ_COMPRAS = "compras.txt";
static const char *ARQ_COMPRAS_ITENS = "compras_itens.txt";
static const char *ARQ_CONTAS_PAGAR = "contas_pagar.txt";

void pers_texto_inicializar(void) {}

void pers_texto_finalizar(void) {}

int pers_texto_salvar_cliente(Cliente c) {
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
        if (lista[i].id == id) {
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
    FILE *f = fopen(ARQ_RECURSOS, "r");
    if (!f) return 0;
    int total = 0; Recurso temp;
    while (fscanf(f, "%d;%119[^;];%59[^;];%d;%lf;%lf\n",
                  &temp.id, temp.descricao, temp.categoria, &temp.quantidade,
                  &temp.preco_custo, &temp.valor_locacao) == 6) {
        if (lista && total < max) {
            lista[total] = temp;
        }
        total++;
    }
    fclose(f);
    if (lista) return (total < max) ? total : max;
    return total;
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
    FILE *f = fopen(ARQ_EQUIPES, "r");
    if (!f) return 0;
    int total = 0; Equipe temp;
    while (fscanf(f, "%d;%99[^;];%19[^;];%59[^;];%lf\n",
                  &temp.id, temp.nome, temp.cpf, temp.funcao, &temp.valor_hora) == 5) {
        if (lista && total < max) {
            lista[total] = temp;
        }
        total++;
    }
    fclose(f);
    if (lista) return (total < max) ? total : max;
    return total;
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

/* ======================= Caixa (Lancamentos) ======================= */
int pers_texto_salvar_caixa(CaixaLancamento l){
    if(l.id<=0){ CaixaLancamento arr[1024]; int n=pers_texto_carregar_caixa(arr,1024); int max_id=0; for(int i=0;i<n;i++) if(arr[i].id>max_id) max_id=arr[i].id; l.id=max_id+1; }
    FILE *f = fopen(ARQ_CAIXA, "a"); if(!f) return 0;
    fprintf(f, "%d;%s;%s;%s;%.2f;%s\n", l.id, l.data, l.tipo, l.forma, l.valor, l.descricao);
    fclose(f); return 1;
}
int pers_texto_carregar_caixa(CaixaLancamento *lista, int max){ FILE *f=fopen(ARQ_CAIXA,"r"); if(!f) return 0; int n=0; CaixaLancamento t; while(n<max && fscanf(f, "%d;%10[^;];%15[^;];%15[^;];%lf;%127[^\n]\n", &t.id,t.data,t.tipo,t.forma,&t.valor,t.descricao)==6){ lista[n++]=t; } fclose(f); return n; }

/* ======================= Contas a Receber ========================== */
int pers_texto_salvar_conta_receber(ContaReceber c){
    ContaReceber *arr=NULL; int n=0; FILE *f=fopen(ARQ_CONTAS_RECEBER,"r"); if(f){ ContaReceber t; while(fscanf(f, "%d;%d;%lf;%lf;%10[^;];%10[^;];%d\n", &t.id,&t.evento_id,&t.valor_fatura,&t.valor_recebido,t.data_emissao,t.data_vencimento,&t.status)==7){ arr=realloc(arr,sizeof(ContaReceber)*(n+1)); if(!arr){fclose(f);return 0;} arr[n++]=t; } fclose(f);} 
    int found=0; if(c.id>0){ for(int i=0;i<n;i++){ if(arr[i].id==c.id){ arr[i]=c; found=1; break; } } }
    if(!found){ if(c.id==0){ int max_id=0; for(int i=0;i<n;i++) if(arr[i].id>max_id) max_id=arr[i].id; c.id=max_id+1; } arr=realloc(arr,sizeof(ContaReceber)*(n+1)); if(!arr) return 0; arr[n++]=c; }
    f=fopen(ARQ_CONTAS_RECEBER,"w"); if(!f){ free(arr); return 0; }
    for(int i=0;i<n;i++) fprintf(f, "%d;%d;%.2f;%.2f;%s;%s;%d\n", arr[i].id, arr[i].evento_id, arr[i].valor_fatura, arr[i].valor_recebido, arr[i].data_emissao, arr[i].data_vencimento, arr[i].status);
    fclose(f); free(arr); return c.id;
}
int pers_texto_carregar_contas_receber(ContaReceber *lista,int max){ FILE *f=fopen(ARQ_CONTAS_RECEBER,"r"); if(!f) return 0; int n=0; while(n<max && fscanf(f, "%d;%d;%lf;%lf;%10[^;];%10[^;];%d\n", &lista[n].id,&lista[n].evento_id,&lista[n].valor_fatura,&lista[n].valor_recebido,lista[n].data_emissao,lista[n].data_vencimento,&lista[n].status)==7){ n++; } fclose(f); return n; }

/* ======================= Compras ================================ */
int pers_texto_salvar_compra(Compra c){
    Compra *arr=NULL; int n=0; FILE *f=fopen(ARQ_COMPRAS,"r"); if(f){ Compra t; while(fscanf(f, "%d;%d;%10[^;];%lf;%lf;%lf;%lf;%d;%lf\n", &t.id,&t.fornecedor_id,t.data,&t.frete,&t.imposto,&t.total_produtos,&t.total_nota,&t.pagamento_tipo,&t.entrada)==9){ arr=realloc(arr,sizeof(Compra)*(n+1)); if(!arr){fclose(f);return 0;} arr[n++]=t; } fclose(f);} 
    int found=0; if(c.id>0){ for(int i=0;i<n;i++){ if(arr[i].id==c.id){ arr[i]=c; found=1; break; } } }
    if(!found){ if(c.id==0){ int max_id=0; for(int i=0;i<n;i++) if(arr[i].id>max_id) max_id=arr[i].id; c.id=max_id+1; } arr=realloc(arr,sizeof(Compra)*(n+1)); if(!arr) return 0; arr[n++]=c; }
    f=fopen(ARQ_COMPRAS,"w"); if(!f){ free(arr); return 0; }
    for(int i=0;i<n;i++) fprintf(f, "%d;%d;%s;%.2f;%.2f;%.2f;%.2f;%d;%.2f\n", arr[i].id, arr[i].fornecedor_id, arr[i].data, arr[i].frete, arr[i].imposto, arr[i].total_produtos, arr[i].total_nota, arr[i].pagamento_tipo, arr[i].entrada);
    fclose(f); free(arr); return c.id;
}
int pers_texto_carregar_compras(Compra *lista,int max){ FILE *f=fopen(ARQ_COMPRAS,"r"); if(!f) return 0; int n=0; while(n<max && fscanf(f, "%d;%d;%10[^;];%lf;%lf;%lf;%lf;%d;%lf\n", &lista[n].id,&lista[n].fornecedor_id,lista[n].data,&lista[n].frete,&lista[n].imposto,&lista[n].total_produtos,&lista[n].total_nota,&lista[n].pagamento_tipo,&lista[n].entrada)==9){ n++; } fclose(f); return n; }

int pers_texto_salvar_compra_item(CompraItem it){
    CompraItem *arr=NULL; int n=0; FILE *f=fopen(ARQ_COMPRAS_ITENS,"r");
    if(f){ CompraItem t; 
        while(1){
            int r = fscanf(f, "%d;%d;%d;%lf;%d;%lf;%lf\n", &t.id,&t.compra_id,&t.recurso_id,&t.preco_custo_unit,&t.quantidade,&t.total_item,&t.preco_locacao_unit);
            if(r==7){ /* linha nova com locacao */ }
            else if(r==6){ t.preco_locacao_unit = 0; }
            else break;
            arr=realloc(arr,sizeof(CompraItem)*(n+1)); if(!arr){fclose(f);return 0;} arr[n++]=t;
        }
        fclose(f);
    }
    int found=0; if(it.id>0){ for(int i=0;i<n;i++){ if(arr[i].id==it.id){ arr[i]=it; found=1; break; } } }
    if(!found){ if(it.id==0){ int max_id=0; for(int i=0;i<n;i++) if(arr[i].id>max_id) max_id=arr[i].id; it.id=max_id+1; } arr=realloc(arr,sizeof(CompraItem)*(n+1)); if(!arr) return 0; arr[n++]=it; }
    f=fopen(ARQ_COMPRAS_ITENS,"w"); if(!f){ free(arr); return 0; }
    for(int i=0;i<n;i++) fprintf(f, "%d;%d;%d;%.2f;%d;%.2f;%.2f\n", arr[i].id, arr[i].compra_id, arr[i].recurso_id, arr[i].preco_custo_unit, arr[i].quantidade, arr[i].total_item, arr[i].preco_locacao_unit);
    fclose(f); free(arr); return it.id;
}
int pers_texto_carregar_compra_itens_por_compra(int compra_id, CompraItem *lista,int max){ FILE *f=fopen(ARQ_COMPRAS_ITENS,"r"); if(!f) return 0; int n=0; CompraItem t; while(n<max){ int r=fscanf(f, "%d;%d;%d;%lf;%d;%lf;%lf\n", &t.id,&t.compra_id,&t.recurso_id,&t.preco_custo_unit,&t.quantidade,&t.total_item,&t.preco_locacao_unit); if(r==7 || r==6){ if(r==6) t.preco_locacao_unit=0; if(t.compra_id==compra_id){ lista[n++]=t; } } else break; } fclose(f); return n; }

/* ======================= Contas a Pagar ========================= */
int pers_texto_salvar_conta_pagar(ContaPagar c){
    ContaPagar *arr=NULL; int n=0; FILE *f=fopen(ARQ_CONTAS_PAGAR,"r"); if(f){ ContaPagar t; while(fscanf(f, "%d;%d;%d;%10[^;];%lf;%d\n", &t.id,&t.fornecedor_id,&t.compra_id,t.data_vencimento,&t.valor,&t.status)==6){ arr=realloc(arr,sizeof(ContaPagar)*(n+1)); if(!arr){fclose(f);return 0;} arr[n++]=t; } fclose(f);} 
    int found=0; if(c.id>0){ for(int i=0;i<n;i++){ if(arr[i].id==c.id){ arr[i]=c; found=1; break; } } }
    if(!found){ if(c.id==0){ int max_id=0; for(int i=0;i<n;i++) if(arr[i].id>max_id) max_id=arr[i].id; c.id=max_id+1; } arr=realloc(arr,sizeof(ContaPagar)*(n+1)); if(!arr) return 0; arr[n++]=c; }
    f=fopen(ARQ_CONTAS_PAGAR,"w"); if(!f){ free(arr); return 0; }
    for(int i=0;i<n;i++) fprintf(f, "%d;%d;%d;%s;%.2f;%d\n", arr[i].id, arr[i].fornecedor_id, arr[i].compra_id, arr[i].data_vencimento, arr[i].valor, arr[i].status);
    fclose(f); free(arr); return c.id;
}
int pers_texto_carregar_contas_pagar(ContaPagar *lista,int max){ FILE *f=fopen(ARQ_CONTAS_PAGAR,"r"); if(!f) return 0; int n=0; while(n<max && fscanf(f, "%d;%d;%d;%10[^;];%lf;%d\n", &lista[n].id,&lista[n].fornecedor_id,&lista[n].compra_id,lista[n].data_vencimento,&lista[n].valor,&lista[n].status)==6){ n++; } fclose(f); return n; }

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

/* ===== Evento ===== */

// Formato esperado: id;nome;cliente_id;operador_id;produtora_id;data_inicio;data_fim;local;tipo;status;total_estimado;total_final
static const char *EVENTO_FORMAT_OUT = "%d;%s;%d;%d;%d;%s;%s;%s;%d;%d;%.2f;%.2f\n";

static int parse_evento_line(const char *line, Evento *out) {
    if (!line || !out) return 0;
    // Copia para buffer mutável
    char buf[512];
    strncpy(buf, line, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';

    // Remove \r\n
    size_t L = strlen(buf);
    while (L>0 && (buf[L-1]=='\n' || buf[L-1]=='\r')) { buf[--L] = '\0'; }

    // Tokenizar pelos ';'
    char *tokens[12];
    int count = 0;
    char *p = buf;
    while (count < 12) {
        char *sep = strchr(p, ';');
        if (sep) {
            *sep = '\0';
            tokens[count++] = p;
            p = sep + 1;
        } else {
            tokens[count++] = p;
            break;
        }
    }
    // Se faltam tokens, falha
    if (count != 12) return 0;

    // Conversões
    out->id = atoi(tokens[0]);
    strncpy(out->nome, tokens[1], sizeof(out->nome)-1); out->nome[sizeof(out->nome)-1]='\0';
    out->cliente_id = atoi(tokens[2]);
    out->operador_id = atoi(tokens[3]);
    out->produtora_id = atoi(tokens[4]);
    strncpy(out->data_inicio, tokens[5], sizeof(out->data_inicio)-1); out->data_inicio[sizeof(out->data_inicio)-1]='\0';
    strncpy(out->data_fim, tokens[6], sizeof(out->data_fim)-1); out->data_fim[sizeof(out->data_fim)-1]='\0';
    strncpy(out->local, tokens[7], sizeof(out->local)-1); out->local[sizeof(out->local)-1]='\0';
    out->tipo = (EventoTipo)atoi(tokens[8]);
    out->status = (EventoStatus)atoi(tokens[9]);
    out->total_estimado = atof(tokens[10]);
    out->total_final = atof(tokens[11]);
    return 1;
}

static int get_next_evento_id(void) {
    FILE *f = fopen(ARQ_EVENTOS, "r");
    if (!f) return 1;

    int max_id = 0;
    Evento temp;
    while (fscanf(f, "%d;%*[^\n]\n", &temp.id) == 1) {
        if (temp.id > max_id) {
            max_id = temp.id;
        }
    }
    fclose(f);
    return max_id + 1;
}

int pers_texto_salvar_evento(Evento e) {
    Evento *lista = NULL;
    int total = 0;
    FILE *f = fopen(ARQ_EVENTOS, "r");
    if (f) {
        char line[512];
        while (fgets(line, sizeof(line), f)) {
            Evento temp;
            if (parse_evento_line(line, &temp)) {
                Evento *novo = realloc(lista, sizeof(Evento)*(total+1));
                if (!novo) { fclose(f); free(lista); return 0; }
                lista = novo; lista[total++] = temp;
            }
        }
        fclose(f);
    }
    // Atualiza ou adiciona
    int found = 0;
    if (e.id > 0) {
        for (int i=0;i<total;i++) {
            if (lista[i].id == e.id) { lista[i] = e; found = 1; break; }
        }
    }
    if (!found) {
        if (e.id == 0) e.id = get_next_evento_id();
        Evento *novo = realloc(lista, sizeof(Evento)*(total+1));
        if (!novo) { free(lista); return 0; }
        lista = novo; lista[total++] = e;
    }
    f = fopen(ARQ_EVENTOS, "w");
    if (!f) { free(lista); return 0; }
    for (int i=0;i<total;i++) {
        fprintf(f, EVENTO_FORMAT_OUT,
            lista[i].id, lista[i].nome, lista[i].cliente_id, lista[i].operador_id,
            lista[i].produtora_id, lista[i].data_inicio, lista[i].data_fim, lista[i].local,
            (int)lista[i].tipo, (int)lista[i].status, lista[i].total_estimado, lista[i].total_final);
    }
    fclose(f); free(lista); return e.id;
}

int pers_texto_carregar_eventos(Evento *lista, int max) {
    FILE *f = fopen(ARQ_EVENTOS, "r");
    if (!f) return 0;
    int count = 0; char line[512];
    while (fgets(line, sizeof(line), f) && count < max) {
        Evento temp;
        if (parse_evento_line(line, &temp)) {
            lista[count++] = temp;
        }
    }
    fclose(f); return count;
}

int pers_texto_remover_evento(int id) {
    const char *path = ARQ_EVENTOS;
    char temp_path[] = "eventos_temp.txt";
    FILE *fin = fopen(path, "r"); if (!fin) return 0;
    FILE *fout = fopen(temp_path, "w"); if (!fout) { fclose(fin); return 0; }
    int found = 0; char line[512];
    while (fgets(line, sizeof(line), fin)) {
        Evento ev; if (!parse_evento_line(line, &ev)) continue;
        if (ev.id == id) { found = 1; continue; }
        fprintf(fout, EVENTO_FORMAT_OUT,
            ev.id, ev.nome, ev.cliente_id, ev.operador_id, ev.produtora_id,
            ev.data_inicio, ev.data_fim, ev.local,
            (int)ev.tipo, (int)ev.status, ev.total_estimado, ev.total_final);
    }
    fclose(fin); fclose(fout);
    if (found) { remove(path); rename(temp_path, path); } else { remove(temp_path); }
    return found;
}

/* ===== EventoItem (itens de recursos por evento) ===== */
int pers_texto_salvar_evento_item(EventoItem it) {
    FILE *f = fopen(ARQ_EVENTOS_ITENS, "r");
    EventoItem *lista = NULL, temp; int total = 0;
    if (f) {
        while (fscanf(f, "%d;%d;%d;%d\n",
                      &temp.id,
                      &temp.evento_id,
                      &temp.recurso_id,
                      &temp.quantidade) == 4) {
            lista = realloc(lista, sizeof(EventoItem) * (total + 1));
            if (!lista) { fclose(f); return 0; }
            lista[total++] = temp;
        }
        fclose(f);
    }
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id == it.id && it.id > 0) { lista[i] = it; found = 1; break; }
    }
    if (!found) {
        int max_id = 0;
        for (int i = 0; i < total; i++) if (lista[i].id > max_id) max_id = lista[i].id;
        it.id = max_id + 1;
        lista = realloc(lista, sizeof(EventoItem) * (total + 1));
        if (!lista) return 0;
        lista[total++] = it;
    }
    f = fopen(ARQ_EVENTOS_ITENS, "w"); if (!f) { free(lista); return 0; }
    for (int i = 0; i < total; i++) {
        fprintf(f, "%d;%d;%d;%d\n",
                lista[i].id,
                lista[i].evento_id,
                lista[i].recurso_id,
                lista[i].quantidade);
    }
    fclose(f); free(lista); return 1;
}

int pers_texto_carregar_evento_itens(EventoItem *lista, int max) {
    FILE *f = fopen(ARQ_EVENTOS_ITENS, "r");
    if (!f) return 0;
    int total = 0; EventoItem temp;
    while (fscanf(f, "%d;%d;%d;%d\n",
                  &temp.id,
                  &temp.evento_id,
                  &temp.recurso_id,
                  &temp.quantidade) == 4) {
        if (lista && total < max)
            lista[total] = temp;
        total++;
    }
    fclose(f);
    if (lista)
        return (total < max) ? total : max;
    return total;
}

int pers_texto_remover_evento_itens_por_evento(int evento_id) {
    FILE *f = fopen(ARQ_EVENTOS_ITENS, "r");
    EventoItem *lista = NULL, temp; int total = 0;
    if (f) {
        while (fscanf(f, "%d;%d;%d;%d\n", &temp.id, &temp.evento_id, &temp.recurso_id, &temp.quantidade) == 4) {
            lista = realloc(lista, sizeof(EventoItem) * (total + 1));
            if (!lista) { fclose(f); return 0; }
            lista[total++] = temp;
        }
        fclose(f);
    }
    
    f = fopen(ARQ_EVENTOS_ITENS, "w"); 
    if (!f) { 
        if (lista) free(lista); 
        return 0; 
    }

    if (lista) {
        for (int i = 0; i < total; i++) {
            if (lista[i].evento_id == evento_id) continue;
            fprintf(f, "%d;%d;%d;%d\n", lista[i].id, lista[i].evento_id, lista[i].recurso_id, lista[i].quantidade);
        }
        free(lista);
    }
    
    fclose(f);
    return 1;
}

/* ===== EventoEquipe (múltiplas equipes por evento) ===== */
int pers_texto_salvar_evento_equipe(EventoEquipe ee) {
    FILE *f = fopen(ARQ_EVENTOS_EQUIPES, "r");
    EventoEquipe *lista = NULL, temp; int total = 0;
    if (f) {
        while (fscanf(f, "%d;%d;%d;%lf\n", &temp.id, &temp.evento_id, &temp.equipe_id, &temp.horas_trabalhadas) == 4) {
            lista = realloc(lista, sizeof(EventoEquipe) * (total + 1));
            if (!lista) { fclose(f); return 0; }
            lista[total++] = temp;
        }
        fclose(f);
    }
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id == ee.id && ee.id > 0) { lista[i] = ee; found = 1; break; }
    }
    if (!found) {
        int max_id = 0;
        for (int i = 0; i < total; i++) if (lista[i].id > max_id) max_id = lista[i].id;
        ee.id = max_id + 1;
        lista = realloc(lista, sizeof(EventoEquipe) * (total + 1));
        if (!lista) return 0;
        lista[total++] = ee;
    }
    f = fopen(ARQ_EVENTOS_EQUIPES, "w"); if (!f) { free(lista); return 0; }
    for (int i = 0; i < total; i++) {
        fprintf(f, "%d;%d;%d;%.2f\n", lista[i].id, lista[i].evento_id, lista[i].equipe_id, lista[i].horas_trabalhadas);
    }
    fclose(f); free(lista); return 1;
}

int pers_texto_carregar_evento_equipes(EventoEquipe *lista, int max) {
    FILE *f = fopen(ARQ_EVENTOS_EQUIPES, "r");
    if (!f) return 0;
    int total = 0; EventoEquipe temp;
    while (fscanf(f, "%d;%d;%d;%lf\n", &temp.id, &temp.evento_id, &temp.equipe_id, &temp.horas_trabalhadas) == 4) {
        if (lista && total < max) lista[total] = temp;
        total++;
    }
    fclose(f);
    if (lista) return (total < max) ? total : max;
    return total;
}

int pers_texto_remover_evento_equipes_por_evento(int evento_id) {
    FILE *f = fopen(ARQ_EVENTOS_EQUIPES, "r");
    EventoEquipe *lista = NULL, temp; int total = 0;
    if (f) {
        while (fscanf(f, "%d;%d;%d;%lf\n", &temp.id, &temp.evento_id, &temp.equipe_id, &temp.horas_trabalhadas) == 4) {
            lista = realloc(lista, sizeof(EventoEquipe) * (total + 1));
            if (!lista) { fclose(f); return 0; }
            lista[total++] = temp;
        }
        fclose(f);
    }
    
    f = fopen(ARQ_EVENTOS_EQUIPES, "w"); 
    if (!f) { 
        if (lista) free(lista); 
        return 0; 
    }

    if (lista) {
        for (int i = 0; i < total; i++) {
            if (lista[i].evento_id == evento_id) continue;
            fprintf(f, "%d;%d;%d;%.2f\n", lista[i].id, lista[i].evento_id, lista[i].equipe_id, lista[i].horas_trabalhadas);
        }
        free(lista);
    }
    
    fclose(f);
    return 1;
}

/* ===== EventoFornecedor (múltiplos fornecedores por evento) ===== */
int pers_texto_salvar_evento_fornecedor(EventoFornecedor ef) {
    FILE *f = fopen(ARQ_EVENTOS_FORNECEDORES, "r");
    EventoFornecedor *lista = NULL, temp; int total = 0;
    if (f) {
        while (fscanf(f, "%d;%d;%d;%lf\n", &temp.id, &temp.evento_id, &temp.fornecedor_id, &temp.valor_servico) == 4) {
            lista = realloc(lista, sizeof(EventoFornecedor) * (total + 1));
            if (!lista) { fclose(f); return 0; }
            lista[total++] = temp;
        }
        fclose(f);
    }
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id == ef.id && ef.id > 0) { lista[i] = ef; found = 1; break; }
    }
    if (!found) {
        int max_id = 0;
        for (int i = 0; i < total; i++) if (lista[i].id > max_id) max_id = lista[i].id;
        ef.id = max_id + 1;
        lista = realloc(lista, sizeof(EventoFornecedor) * (total + 1));
        if (!lista) return 0;
        lista[total++] = ef;
    }
    f = fopen(ARQ_EVENTOS_FORNECEDORES, "w"); if (!f) { free(lista); return 0; }
    for (int i = 0; i < total; i++) {
        fprintf(f, "%d;%d;%d;%.2f\n", lista[i].id, lista[i].evento_id, lista[i].fornecedor_id, lista[i].valor_servico);
    }
    fclose(f); free(lista); return 1;
}

int pers_texto_carregar_evento_fornecedores(EventoFornecedor *lista, int max) {
    FILE *f = fopen(ARQ_EVENTOS_FORNECEDORES, "r");
    if (!f) return 0;
    int total = 0; EventoFornecedor temp;
    while (fscanf(f, "%d;%d;%d;%lf\n", &temp.id, &temp.evento_id, &temp.fornecedor_id, &temp.valor_servico) == 4) {
        if (lista && total < max) lista[total] = temp;
        total++;
    }
    fclose(f);
    if (lista) return (total < max) ? total : max;
    return total;
}

int pers_texto_remover_evento_fornecedores_por_evento(int evento_id) {
    FILE *f = fopen(ARQ_EVENTOS_FORNECEDORES, "r");
    EventoFornecedor *lista = NULL, temp; int total = 0;
    if (f) {
        while (fscanf(f, "%d;%d;%d;%lf\n", &temp.id, &temp.evento_id, &temp.fornecedor_id, &temp.valor_servico) == 4) {
            lista = realloc(lista, sizeof(EventoFornecedor) * (total + 1));
            if (!lista) { fclose(f); return 0; }
            lista[total++] = temp;
        }
        fclose(f);
    }
    
    f = fopen(ARQ_EVENTOS_FORNECEDORES, "w"); 
    if (!f) { 
        if (lista) free(lista); 
        return 0; 
    }

    if (lista) {
        for (int i = 0; i < total; i++) {
            if (lista[i].evento_id == evento_id) continue;
            fprintf(f, "%d;%d;%d;%.2f\n", lista[i].id, lista[i].evento_id, lista[i].fornecedor_id, lista[i].valor_servico);
        }
        free(lista);
    }
    
    fclose(f);
    return 1;
}