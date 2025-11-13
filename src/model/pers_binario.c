#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pers_binario.h"

static const char *ARQ_CLIENTES_BIN = "clientes.bin";
static const char *ARQ_RECURSOS_BIN = "recursos.bin";
static const char *ARQ_EQUIPES_BIN  = "equipes.bin";
static const char *ARQ_FORNEC_BIN   = "fornecedores.bin";
static const char *ARQ_OPER_BIN     = "operadores.bin";
static const char *ARQ_PROD_BIN     = "produtora.bin";

/* formato: sequência de registros Cliente com tamanho fixo (struct) */
/* NOTE: use fwrite/fread com structs com campos char[] fixos (como o nosso Cliente) */

void pers_binario_inicializar(void) {
    /* nada especial */
}

void pers_binario_finalizar(void) {
    /* nada especial */
}

int pers_binario_salvar_cliente(Cliente c) {
    /* carregar todos, substituir se id existe, senão append e gravar novamente */
    FILE *f = fopen(ARQ_CLIENTES_BIN, "rb");
    Cliente *lista = NULL;
    int total = 0;
    if (f) {
        Cliente temp;
        while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
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
    f = fopen(ARQ_CLIENTES_BIN, "wb");
    if (!f) { free(lista); return 0; }
    fwrite(lista, sizeof(Cliente), total, f);
    fclose(f);
    free(lista);
    return 1;
}

int pers_binario_carregar_clientes(Cliente *lista, int max) {
    FILE *f = fopen(ARQ_CLIENTES_BIN, "rb");
    if (!f) return 0;
    int total = 0;
    Cliente temp;
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
        if (lista && total < max) lista[total] = temp;
        total++;
    }
    fclose(f);
    if (lista) return (total < max) ? total : max;
    return total;
}

int pers_binario_remover_cliente(int id) {
    FILE *f = fopen(ARQ_CLIENTES_BIN, "rb");
    if (!f) return 0;
    Cliente *lista = NULL;
    int total = 0;
    Cliente temp;
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
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
    f = fopen(ARQ_CLIENTES_BIN, "wb");
    if (!f) { free(lista); return 0; }
    fwrite(lista, sizeof(Cliente), total, f);
    fclose(f);
    free(lista);
    return 1;
}

/* ===== Recurso ===== */
int pers_binario_salvar_recurso(Recurso r) {
    FILE *f=fopen(ARQ_RECURSOS_BIN,"rb"); Recurso *lista=NULL, temp; int total=0;
    if(f){ while(fread(&temp,sizeof(Recurso),1,f)==1){ lista=realloc(lista,sizeof(Recurso)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp; } fclose(f);} 
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==r.id){lista[i]=r;found=1;break;} }
    if(!found){ lista=realloc(lista,sizeof(Recurso)*(total+1)); if(!lista) return 0; lista[total++]=r; }
    f=fopen(ARQ_RECURSOS_BIN,"wb"); if(!f){ free(lista); return 0; }
    fwrite(lista,sizeof(Recurso),total,f); fclose(f); free(lista); return 1;
}
int pers_binario_carregar_recursos(Recurso *lista,int max){ FILE *f=fopen(ARQ_RECURSOS_BIN,"rb"); if(!f) return 0; int total=0; Recurso temp; while(fread(&temp,sizeof(Recurso),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; } fclose(f); if(lista) return (total<max)?total:max; return total; }
int pers_binario_remover_recurso(int id){ FILE *f=fopen(ARQ_RECURSOS_BIN,"rb"); if(!f) return 0; Recurso *lista=NULL,temp; int total=0; while(fread(&temp,sizeof(Recurso),1,f)==1){ lista=realloc(lista,sizeof(Recurso)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f); int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } } if(!found){ free(lista); return 0; } f=fopen(ARQ_RECURSOS_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Recurso),total,f); fclose(f); free(lista); return 1; }

/* ===== Equipe ===== */
int pers_binario_salvar_equipe(Equipe e){ FILE *f=fopen(ARQ_EQUIPES_BIN,"rb"); Equipe *lista=NULL,temp; int total=0; if(f){ while(fread(&temp,sizeof(Equipe),1,f)==1){ lista=realloc(lista,sizeof(Equipe)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);} int found=0; for(int i=0;i<total;i++){ if(lista[i].id==e.id){lista[i]=e;found=1;break;} } if(!found){ lista=realloc(lista,sizeof(Equipe)*(total+1)); if(!lista) return 0; lista[total++]=e; } f=fopen(ARQ_EQUIPES_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Equipe),total,f); fclose(f); free(lista); return 1; }
int pers_binario_carregar_equipes(Equipe *lista,int max){ FILE *f=fopen(ARQ_EQUIPES_BIN,"rb"); if(!f) return 0; int total=0; Equipe temp; while(fread(&temp,sizeof(Equipe),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; } fclose(f); if(lista) return (total<max)?total:max; return total; }
int pers_binario_remover_equipe(int id){ FILE *f=fopen(ARQ_EQUIPES_BIN,"rb"); if(!f) return 0; Equipe *lista=NULL,temp; int total=0; while(fread(&temp,sizeof(Equipe),1,f)==1){ lista=realloc(lista,sizeof(Equipe)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f); int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } } if(!found){ free(lista); return 0; } f=fopen(ARQ_EQUIPES_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Equipe),total,f); fclose(f); free(lista); return 1; }

/* ===== Fornecedor ===== */
int pers_binario_salvar_fornecedor(Fornecedor fz){ FILE *f=fopen(ARQ_FORNEC_BIN,"rb"); Fornecedor *lista=NULL,temp; int total=0; if(f){ while(fread(&temp,sizeof(Fornecedor),1,f)==1){ lista=realloc(lista,sizeof(Fornecedor)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);} int found=0; for(int i=0;i<total;i++){ if(lista[i].id==fz.id){lista[i]=fz;found=1;break;} } if(!found){ lista=realloc(lista,sizeof(Fornecedor)*(total+1)); if(!lista) return 0; lista[total++]=fz; } f=fopen(ARQ_FORNEC_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Fornecedor),total,f); fclose(f); free(lista); return 1; }
int pers_binario_carregar_fornecedores(Fornecedor *lista,int max){ FILE *f=fopen(ARQ_FORNEC_BIN,"rb"); if(!f) return 0; int total=0; Fornecedor temp; while(fread(&temp,sizeof(Fornecedor),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; } fclose(f); if(lista) return (total<max)?total:max; return total; }
int pers_binario_remover_fornecedor(int id){ FILE *f=fopen(ARQ_FORNEC_BIN,"rb"); if(!f) return 0; Fornecedor *lista=NULL,temp; int total=0; while(fread(&temp,sizeof(Fornecedor),1,f)==1){ lista=realloc(lista,sizeof(Fornecedor)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f); int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } } if(!found){ free(lista); return 0; } f=fopen(ARQ_FORNEC_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Fornecedor),total,f); fclose(f); free(lista); return 1; }

/* ===== Operador ===== */
int pers_binario_salvar_operador(Operador o){ FILE *f=fopen(ARQ_OPER_BIN,"rb"); Operador *lista=NULL,temp; int total=0; if(f){ while(fread(&temp,sizeof(Operador),1,f)==1){ lista=realloc(lista,sizeof(Operador)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);} int found=0; for(int i=0;i<total;i++){ if(lista[i].id==o.id){lista[i]=o;found=1;break;} } if(!found){ lista=realloc(lista,sizeof(Operador)*(total+1)); if(!lista) return 0; lista[total++]=o; } f=fopen(ARQ_OPER_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Operador),total,f); fclose(f); free(lista); return 1; }
int pers_binario_carregar_operadores(Operador *lista,int max){ FILE *f=fopen(ARQ_OPER_BIN,"rb"); if(!f) return 0; int total=0; Operador temp; while(fread(&temp,sizeof(Operador),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; } fclose(f); if(lista) return (total<max)?total:max; return total; }
int pers_binario_remover_operador(int id){ FILE *f=fopen(ARQ_OPER_BIN,"rb"); if(!f) return 0; Operador *lista=NULL,temp; int total=0; while(fread(&temp,sizeof(Operador),1,f)==1){ lista=realloc(lista,sizeof(Operador)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f); int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } } if(!found){ free(lista); return 0; } f=fopen(ARQ_OPER_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Operador),total,f); fclose(f); free(lista); return 1; }

/* ===== Produtora (única) ===== */
int pers_binario_salvar_produtora(Produtora p){ FILE *f=fopen(ARQ_PROD_BIN,"wb"); if(!f) return 0; fwrite(&p,sizeof(Produtora),1,f); fclose(f); return 1; }
int pers_binario_obter_produtora(Produtora *out){ if(!out) return 0; FILE *f=fopen(ARQ_PROD_BIN,"rb"); if(!f) return 0; int ok=fread(out,sizeof(Produtora),1,f)==1; fclose(f); return ok; }