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
static const char *ARQ_EVENTOS_BIN  = "eventos.bin";
static const char *ARQ_CAIXA_BIN    = "caixa.bin";
static const char *ARQ_CONTAS_RECEBER_BIN = "contas_receber.bin";
static const char *ARQ_COMPRAS_BIN  = "compras.bin";
static const char *ARQ_COMPRA_ITENS_BIN = "compras_itens.bin";
static const char *ARQ_CONTAS_PAGAR_BIN = "contas_pagar.bin";

void pers_binario_inicializar(void) {}

void pers_binario_finalizar(void) {}

int pers_binario_salvar_cliente(Cliente c) {
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
int pers_binario_remover_produtora(void){ return remove(ARQ_PROD_BIN)==0 ? 1 : 0; }

/* ===== Evento ===== */
int pers_binario_salvar_evento(Evento e){ FILE *f=fopen(ARQ_EVENTOS_BIN,"rb"); Evento *lista=NULL,temp; int total=0; if(f){ while(fread(&temp,sizeof(Evento),1,f)==1){ lista=realloc(lista,sizeof(Evento)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);} int found=0; for(int i=0;i<total;i++){ if(lista[i].id==e.id){lista[i]=e;found=1;break;} } if(!found){ lista=realloc(lista,sizeof(Evento)*(total+1)); if(!lista) return 0; lista[total++]=e; } f=fopen(ARQ_EVENTOS_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Evento),total,f); fclose(f); free(lista); return 1; }
int pers_binario_carregar_eventos(Evento *lista,int max){ FILE *f=fopen(ARQ_EVENTOS_BIN,"rb"); if(!f) return 0; int total=0; Evento temp; while(fread(&temp,sizeof(Evento),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; } fclose(f); if(lista) return (total<max)?total:max; return total; }
int pers_binario_remover_evento(int id){ FILE *f=fopen(ARQ_EVENTOS_BIN,"rb"); if(!f) return 0; Evento *lista=NULL,temp; int total=0; while(fread(&temp,sizeof(Evento),1,f)==1){ lista=realloc(lista,sizeof(Evento)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f); int found=0; for(int i=0;i<total;i++){ if(lista[i].id==id){ for(int j=i;j<total-1;j++) lista[j]=lista[j+1]; total--; found=1; break; } } if(!found){ free(lista); return 0; } f=fopen(ARQ_EVENTOS_BIN,"wb"); if(!f){ free(lista); return 0; } fwrite(lista,sizeof(Evento),total,f); fclose(f); free(lista); return 1; }

int pers_binario_salvar_evento_equipe(EventoEquipe ee) { (void)ee; return 0; }
int pers_binario_carregar_evento_equipes(EventoEquipe *lista, int max) { (void)lista; (void)max; return 0; }
int pers_binario_remover_evento_equipes_por_evento(int evento_id) { (void)evento_id; return 0; }

int pers_binario_salvar_evento_fornecedor(EventoFornecedor ef) { (void)ef; return 0; }
int pers_binario_carregar_evento_fornecedores(EventoFornecedor *lista, int max) { (void)lista; (void)max; return 0; }
int pers_binario_remover_evento_fornecedores_por_evento(int evento_id) { (void)evento_id; return 0; }

/* ==================== Transações em Binário ==================== */

int pers_binario_salvar_caixa(CaixaLancamento l){
    FILE *f=fopen(ARQ_CAIXA_BIN,"rb"); CaixaLancamento *lista=NULL,temp; int total=0;
    if(f){ while(fread(&temp,sizeof(CaixaLancamento),1,f)==1){ lista=realloc(lista,sizeof(CaixaLancamento)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);}
    if(l.id<=0){ int max_id=0; for(int i=0;i<total;i++) if(lista[i].id>max_id) max_id=lista[i].id; l.id=max_id+1; }
    lista=realloc(lista,sizeof(CaixaLancamento)*(total+1)); if(!lista) return 0; lista[total++]=l;
    f=fopen(ARQ_CAIXA_BIN,"wb"); if(!f){ free(lista); return 0; }
    fwrite(lista,sizeof(CaixaLancamento),total,f); fclose(f); free(lista); return l.id;
}
int pers_binario_carregar_caixa(CaixaLancamento *lista, int max){
    FILE *f=fopen(ARQ_CAIXA_BIN,"rb"); if(!f) return 0; int total=0; CaixaLancamento temp;
    while(fread(&temp,sizeof(CaixaLancamento),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; }
    fclose(f); if(lista) return (total<max)?total:max; return total;
}

int pers_binario_salvar_conta_receber(ContaReceber c){
    FILE *f=fopen(ARQ_CONTAS_RECEBER_BIN,"rb"); ContaReceber *lista=NULL,temp; int total=0;
    if(f){ while(fread(&temp,sizeof(ContaReceber),1,f)==1){ lista=realloc(lista,sizeof(ContaReceber)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);}
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==c.id){lista[i]=c;found=1;break;} }
    if(!found){ if(c.id<=0){ int max_id=0; for(int i=0;i<total;i++) if(lista[i].id>max_id) max_id=lista[i].id; c.id=max_id+1; } lista=realloc(lista,sizeof(ContaReceber)*(total+1)); if(!lista) return 0; lista[total++]=c; }
    f=fopen(ARQ_CONTAS_RECEBER_BIN,"wb"); if(!f){ free(lista); return 0; }
    fwrite(lista,sizeof(ContaReceber),total,f); fclose(f); free(lista); return c.id;
}
int pers_binario_carregar_contas_receber(ContaReceber *lista, int max){
    FILE *f=fopen(ARQ_CONTAS_RECEBER_BIN,"rb"); if(!f) return 0; int total=0; ContaReceber temp;
    while(fread(&temp,sizeof(ContaReceber),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; }
    fclose(f); if(lista) return (total<max)?total:max; return total;
}

int pers_binario_salvar_compra(Compra c){
    FILE *f=fopen(ARQ_COMPRAS_BIN,"rb"); Compra *lista=NULL,temp; int total=0;
    if(f){ while(fread(&temp,sizeof(Compra),1,f)==1){ lista=realloc(lista,sizeof(Compra)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);}
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==c.id){lista[i]=c;found=1;break;} }
    if(!found){ if(c.id<=0){ int max_id=0; for(int i=0;i<total;i++) if(lista[i].id>max_id) max_id=lista[i].id; c.id=max_id+1; } lista=realloc(lista,sizeof(Compra)*(total+1)); if(!lista) return 0; lista[total++]=c; }
    f=fopen(ARQ_COMPRAS_BIN,"wb"); if(!f){ free(lista); return 0; }
    fwrite(lista,sizeof(Compra),total,f); fclose(f); free(lista); return c.id;
}
int pers_binario_carregar_compras(Compra *lista, int max){
    FILE *f=fopen(ARQ_COMPRAS_BIN,"rb"); if(!f) return 0; int total=0; Compra temp;
    while(fread(&temp,sizeof(Compra),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; }
    fclose(f); if(lista) return (total<max)?total:max; return total;
}

int pers_binario_salvar_compra_item(CompraItem it){
    FILE *f=fopen(ARQ_COMPRA_ITENS_BIN,"rb"); CompraItem *lista=NULL,temp; int total=0;
    if(f){ while(fread(&temp,sizeof(CompraItem),1,f)==1){ lista=realloc(lista,sizeof(CompraItem)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);}
    int found=0; if(it.id>0){ for(int i=0;i<total;i++){ if(lista[i].id==it.id){lista[i]=it;found=1;break;} } }
    if(!found){ if(it.id<=0){ int max_id=0; for(int i=0;i<total;i++) if(lista[i].id>max_id) max_id=lista[i].id; it.id=max_id+1; } lista=realloc(lista,sizeof(CompraItem)*(total+1)); if(!lista) return 0; lista[total++]=it; }
    f=fopen(ARQ_COMPRA_ITENS_BIN,"wb"); if(!f){ free(lista); return 0; }
    fwrite(lista,sizeof(CompraItem),total,f); fclose(f); free(lista); return it.id;
}
int pers_binario_carregar_compra_itens_por_compra(int compra_id, CompraItem *lista, int max){
    FILE *f=fopen(ARQ_COMPRA_ITENS_BIN,"rb"); if(!f) return 0; int n=0; CompraItem temp;
    while(fread(&temp,sizeof(CompraItem),1,f)==1){ if(temp.compra_id==compra_id){ if(lista && n<max) lista[n]=temp; n++; } }
    fclose(f); return (lista && n>max)?max:n;
}

int pers_binario_salvar_conta_pagar(ContaPagar c){
    FILE *f=fopen(ARQ_CONTAS_PAGAR_BIN,"rb"); ContaPagar *lista=NULL,temp; int total=0;
    if(f){ while(fread(&temp,sizeof(ContaPagar),1,f)==1){ lista=realloc(lista,sizeof(ContaPagar)*(total+1)); if(!lista){fclose(f);return 0;} lista[total++]=temp;} fclose(f);}
    int found=0; for(int i=0;i<total;i++){ if(lista[i].id==c.id){lista[i]=c;found=1;break;} }
    if(!found){ if(c.id<=0){ int max_id=0; for(int i=0;i<total;i++) if(lista[i].id>max_id) max_id=lista[i].id; c.id=max_id+1; } lista=realloc(lista,sizeof(ContaPagar)*(total+1)); if(!lista) return 0; lista[total++]=c; }
    f=fopen(ARQ_CONTAS_PAGAR_BIN,"wb"); if(!f){ free(lista); return 0; }
    fwrite(lista,sizeof(ContaPagar),total,f); fclose(f); free(lista); return c.id;
}
int pers_binario_carregar_contas_pagar(ContaPagar *lista, int max){
    FILE *f=fopen(ARQ_CONTAS_PAGAR_BIN,"rb"); if(!f) return 0; int total=0; ContaPagar temp;
    while(fread(&temp,sizeof(ContaPagar),1,f)==1){ if(lista && total<max) lista[total]=temp; total++; }
    fclose(f); if(lista) return (total<max)?total:max; return total;
}