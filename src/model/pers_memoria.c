#include <string.h>
#include "pers_memoria.h"
#include "evento_item.h"
#include "evento_equipe.h"
#include "evento_fornecedor.h"
#include "transacoes.h"

#define MEM_MAX_CLIENTES 200
#define MEM_MAX_RECURSOS 200
#define MEM_MAX_EQUIPES  200
#define MEM_MAX_FORNEC   200
#define MEM_MAX_OPER     200

static Cliente clientes_mem[MEM_MAX_CLIENTES];
static int total_clientes_mem = 0;

static Recurso recursos_mem[MEM_MAX_RECURSOS];
static int total_recursos_mem = 0;

static Equipe equipes_mem[MEM_MAX_EQUIPES];
static int total_equipes_mem = 0;

static Fornecedor fornecedores_mem[MEM_MAX_FORNEC];
static int total_fornecedores_mem = 0;

static Operador operadores_mem[MEM_MAX_OPER];
static int total_operadores_mem = 0;

#define MEM_MAX_EVENTOS 200
static Evento eventos_mem[MEM_MAX_EVENTOS];
static int total_eventos_mem = 0;

#define MEM_MAX_EVENTO_ITENS 1000
static EventoItem evento_itens_mem[MEM_MAX_EVENTO_ITENS];
static int total_evento_itens_mem = 0;

#define MEM_MAX_EVENTO_EQUIPES 500
static EventoEquipe evento_equipes_mem[MEM_MAX_EVENTO_EQUIPES];
static int total_evento_equipes_mem = 0;

#define MEM_MAX_EVENTO_FORNECEDORES 500
static EventoFornecedor evento_fornecedores_mem[MEM_MAX_EVENTO_FORNECEDORES];
static int total_evento_fornecedores_mem = 0;

static Produtora produtora_mem; /* cadastro único */
static int produtora_definida = 0;

#define MEM_MAX_CAIXA 2000
static CaixaLancamento caixa_mem[MEM_MAX_CAIXA];
static int total_caixa_mem = 0;

#define MEM_MAX_CONTAS_RECEBER 500
static ContaReceber contas_receber_mem[MEM_MAX_CONTAS_RECEBER];
static int total_contas_receber_mem = 0;

#define MEM_MAX_COMPRAS 500
static Compra compras_mem[MEM_MAX_COMPRAS];
static int total_compras_mem = 0;

#define MEM_MAX_COMPRA_ITENS 2000
static CompraItem compra_itens_mem[MEM_MAX_COMPRA_ITENS];
static int total_compra_itens_mem = 0;

#define MEM_MAX_CONTAS_PAGAR 1000
static ContaPagar contas_pagar_mem[MEM_MAX_CONTAS_PAGAR];
static int total_contas_pagar_mem = 0;

void pers_memoria_inicializar(void) {
    total_clientes_mem = 0;
    total_recursos_mem = 0;
    total_equipes_mem = 0;
    total_fornecedores_mem = 0;
    total_operadores_mem = 0;
    produtora_definida = 0;
    total_eventos_mem = 0;
    total_evento_itens_mem = 0;
    total_evento_equipes_mem = 0;
    total_evento_fornecedores_mem = 0;
    total_caixa_mem = 0;
    total_contas_receber_mem = 0;
    total_compras_mem = 0;
    total_compra_itens_mem = 0;
    total_contas_pagar_mem = 0;
}

void pers_memoria_finalizar(void) {
    total_clientes_mem = 0;
    total_recursos_mem = 0;
    total_equipes_mem = 0;
    total_fornecedores_mem = 0;
    total_operadores_mem = 0;
    produtora_definida = 0;
    total_eventos_mem = 0;
    total_evento_itens_mem = 0;
    total_evento_equipes_mem = 0;
    total_evento_fornecedores_mem = 0;
    total_caixa_mem = 0;
    total_contas_receber_mem = 0;
    total_compras_mem = 0;
    total_compra_itens_mem = 0;
    total_contas_pagar_mem = 0;
}

int pers_memoria_salvar_cliente(Cliente c) {
    for (int i = 0; i < total_clientes_mem; i++) {
        if (clientes_mem[i].id == c.id) {
            return 1;
        }
    }
    if (total_clientes_mem >= MEM_MAX_CLIENTES) return 0;
    clientes_mem[total_clientes_mem++] = c;
    return 1;
}

int pers_memoria_carregar_clientes(Cliente *lista, int max) {
    int n = (total_clientes_mem < max) ? total_clientes_mem : max;
    for (int i = 0; i < n; i++) lista[i] = clientes_mem[i];
    return n;
}

int pers_memoria_remover_cliente(int id) {
    for (int i = 0; i < total_clientes_mem; i++) {
        if (clientes_mem[i].id == id) {
            for (int j = i; j < total_clientes_mem - 1; j++)
                clientes_mem[j] = clientes_mem[j + 1];
            total_clientes_mem--;
            return 1;
        }
    }
    return 0;
}

/* ===== Recurso ===== */
int pers_memoria_salvar_recurso(Recurso r) {
    for (int i = 0; i < total_recursos_mem; i++) {
        if (recursos_mem[i].id == r.id) { recursos_mem[i] = r; return 1; }
    }
    if (r.id <= 0 || total_recursos_mem >= MEM_MAX_RECURSOS) return 0;
    recursos_mem[total_recursos_mem++] = r; return 1;
}

int pers_memoria_carregar_recursos(Recurso *lista, int max) {
    int n = (total_recursos_mem < max) ? total_recursos_mem : max;
    for (int i = 0; i < n; i++) lista[i] = recursos_mem[i];
    return n;
}

int pers_memoria_remover_recurso(int id) {
    for (int i = 0; i < total_recursos_mem; i++) {
        if (recursos_mem[i].id == id) {
            for (int j = i; j < total_recursos_mem - 1; j++) recursos_mem[j] = recursos_mem[j + 1];
            total_recursos_mem--; return 1;
        }
    }
    return 0;
}

/* ===== Equipe ===== */
int pers_memoria_salvar_equipe(Equipe e) {
    for (int i = 0; i < total_equipes_mem; i++) {
        if (equipes_mem[i].id == e.id) { equipes_mem[i] = e; return 1; }
    }
    if (e.id <= 0 || total_equipes_mem >= MEM_MAX_EQUIPES) return 0;
    equipes_mem[total_equipes_mem++] = e; return 1;
}

int pers_memoria_carregar_equipes(Equipe *lista, int max) {
    int n = (total_equipes_mem < max) ? total_equipes_mem : max;
    for (int i = 0; i < n; i++) lista[i] = equipes_mem[i];
    return n;
}

int pers_memoria_remover_equipe(int id) {
    for (int i = 0; i < total_equipes_mem; i++) {
        if (equipes_mem[i].id == id) {
            for (int j = i; j < total_equipes_mem - 1; j++) equipes_mem[j] = equipes_mem[j + 1];
            total_equipes_mem--; return 1;
        }
    }
    return 0;
}

/* ===== Fornecedor ===== */
int pers_memoria_salvar_fornecedor(Fornecedor f) {
    for (int i = 0; i < total_fornecedores_mem; i++) {
        if (fornecedores_mem[i].id == f.id) { fornecedores_mem[i] = f; return 1; }
    }
    if (f.id <= 0 || total_fornecedores_mem >= MEM_MAX_FORNEC) return 0;
    fornecedores_mem[total_fornecedores_mem++] = f; return 1;
}

int pers_memoria_carregar_fornecedores(Fornecedor *lista, int max) {
    int n = (total_fornecedores_mem < max) ? total_fornecedores_mem : max;
    for (int i = 0; i < n; i++) lista[i] = fornecedores_mem[i];
    return n;
}

int pers_memoria_remover_fornecedor(int id) {
    for (int i = 0; i < total_fornecedores_mem; i++) {
        if (fornecedores_mem[i].id == id) {
            for (int j = i; j < total_fornecedores_mem - 1; j++) fornecedores_mem[j] = fornecedores_mem[j + 1];
            total_fornecedores_mem--; return 1;
        }
    }
    return 0;
}

/* ===== Operador ===== */
int pers_memoria_salvar_operador(Operador o) {
    for (int i = 0; i < total_operadores_mem; i++) {
        if (operadores_mem[i].id == o.id) { operadores_mem[i] = o; return 1; }
    }
    if (o.id <= 0 || total_operadores_mem >= MEM_MAX_OPER) return 0;
    operadores_mem[total_operadores_mem++] = o; return 1;
}

int pers_memoria_carregar_operadores(Operador *lista, int max) {
    int n = (total_operadores_mem < max) ? total_operadores_mem : max;
    for (int i = 0; i < n; i++) lista[i] = operadores_mem[i];
    return n;
}

int pers_memoria_remover_operador(int id) {
    for (int i = 0; i < total_operadores_mem; i++) {
        if (operadores_mem[i].id == id) {
            for (int j = i; j < total_operadores_mem - 1; j++) operadores_mem[j] = operadores_mem[j + 1];
            total_operadores_mem--; return 1;
        }
    }
    return 0;
}

/* ===== Produtora (única) ===== */
int pers_memoria_salvar_produtora(Produtora p) {
    produtora_mem = p; produtora_definida = 1; return 1;
}

int pers_memoria_obter_produtora(Produtora *out) {
    if (!produtora_definida || !out) return 0;
    *out = produtora_mem; return 1;
}

int pers_memoria_remover_produtora(void) {
    if (!produtora_definida) return 0;
    produtora_definida = 0;
    memset(&produtora_mem, 0, sizeof(Produtora));
    return 1;
}

/* ===== Evento ===== */
int pers_memoria_salvar_evento(Evento e) {
    for (int i = 0; i < total_eventos_mem; i++) {
        if (eventos_mem[i].id == e.id) { eventos_mem[i] = e; return 1; }
    }
    if (e.id <= 0 || total_eventos_mem >= MEM_MAX_EVENTOS) return 0;
    eventos_mem[total_eventos_mem++] = e; return 1;
}

int pers_memoria_carregar_eventos(Evento *lista, int max) {
    int n = (total_eventos_mem < max) ? total_eventos_mem : max;
    for (int i = 0; i < n; i++) lista[i] = eventos_mem[i];
    return n;
}

int pers_memoria_remover_evento(int id) {
    for (int i = 0; i < total_eventos_mem; i++) {
        if (eventos_mem[i].id == id) {
            for (int j = i; j < total_eventos_mem - 1; j++) eventos_mem[j] = eventos_mem[j + 1];
            total_eventos_mem--; return 1;
        }
    }
    return 0;
}

/* ===== EventoItem (stubs em memória) ===== */
int pers_memoria_salvar_evento_item(EventoItem it) {
    for (int i = 0; i < total_evento_itens_mem; i++) {
        if (evento_itens_mem[i].id == it.id && it.id > 0) { evento_itens_mem[i] = it; return 1; }
    }
    if (total_evento_itens_mem >= MEM_MAX_EVENTO_ITENS) return 0;
    int max_id = 0; for(int i=0;i<total_evento_itens_mem;i++) if(evento_itens_mem[i].id > max_id) max_id = evento_itens_mem[i].id;
    it.id = max_id + 1;
    evento_itens_mem[total_evento_itens_mem++] = it; return 1;
}
int pers_memoria_carregar_evento_itens(EventoItem *lista, int max) {
    int n = (total_evento_itens_mem < max) ? total_evento_itens_mem : max;
    for (int i = 0; i < n; i++) lista[i] = evento_itens_mem[i];
    return n;
}
int pers_memoria_remover_evento_itens_por_evento(int evento_id) {
    int i = 0;
    while(i < total_evento_itens_mem){
        if(evento_itens_mem[i].evento_id == evento_id){
            for(int j=i; j < total_evento_itens_mem - 1; j++) evento_itens_mem[j] = evento_itens_mem[j+1];
            total_evento_itens_mem--;
        } else {
            i++;
        }
    }
    return 1;
}

/* ===== EventoEquipe (stubs em memória) ===== */
int pers_memoria_salvar_evento_equipe(EventoEquipe ee) {
    for (int i = 0; i < total_evento_equipes_mem; i++) {
        if (evento_equipes_mem[i].id == ee.id && ee.id > 0) { evento_equipes_mem[i] = ee; return 1; }
    }
    if (total_evento_equipes_mem >= MEM_MAX_EVENTO_EQUIPES) return 0;
    int max_id = 0; for(int i=0;i<total_evento_equipes_mem;i++) if(evento_equipes_mem[i].id > max_id) max_id = evento_equipes_mem[i].id;
    ee.id = max_id + 1;
    evento_equipes_mem[total_evento_equipes_mem++] = ee; return 1;
}
int pers_memoria_carregar_evento_equipes(EventoEquipe *lista, int max) {
    int n = (total_evento_equipes_mem < max) ? total_evento_equipes_mem : max;
    for (int i = 0; i < n; i++) lista[i] = evento_equipes_mem[i];
    return n;
}
int pers_memoria_remover_evento_equipes_por_evento(int evento_id) {
    int i = 0;
    while(i < total_evento_equipes_mem){
        if(evento_equipes_mem[i].evento_id == evento_id){
            for(int j=i; j < total_evento_equipes_mem - 1; j++) evento_equipes_mem[j] = evento_equipes_mem[j+1];
            total_evento_equipes_mem--;
        } else {
            i++;
        }
    }
    return 1;
}

/* ===== EventoFornecedor (stubs em memória) ===== */
int pers_memoria_salvar_evento_fornecedor(EventoFornecedor ef) {
    for (int i = 0; i < total_evento_fornecedores_mem; i++) {
        if (evento_fornecedores_mem[i].id == ef.id && ef.id > 0) { evento_fornecedores_mem[i] = ef; return 1; }
    }
    if (total_evento_fornecedores_mem >= MEM_MAX_EVENTO_FORNECEDORES) return 0;
    int max_id = 0; for(int i=0;i<total_evento_fornecedores_mem;i++) if(evento_fornecedores_mem[i].id > max_id) max_id = evento_fornecedores_mem[i].id;
    ef.id = max_id + 1;
    evento_fornecedores_mem[total_evento_fornecedores_mem++] = ef; return 1;
}
int pers_memoria_carregar_evento_fornecedores(EventoFornecedor *lista, int max) {
    int n = (total_evento_fornecedores_mem < max) ? total_evento_fornecedores_mem : max;
    for (int i = 0; i < n; i++) lista[i] = evento_fornecedores_mem[i];
    return n;
}
int pers_memoria_remover_evento_fornecedores_por_evento(int evento_id) {
    int i = 0;
    while(i < total_evento_fornecedores_mem){
        if(evento_fornecedores_mem[i].evento_id == evento_id){
            for(int j=i; j < total_evento_fornecedores_mem - 1; j++) evento_fornecedores_mem[j] = evento_fornecedores_mem[j+1];
            total_evento_fornecedores_mem--;
        } else {
            i++;
        }
    }
    return 1;
}

/* ==================== Transações em Memória ==================== */

int pers_memoria_salvar_caixa(CaixaLancamento l){
    if(l.id<=0){ int max_id=0; for(int i=0;i<total_caixa_mem;i++) if(caixa_mem[i].id>max_id) max_id=caixa_mem[i].id; l.id=max_id+1; }
    if(total_caixa_mem >= MEM_MAX_CAIXA) return 0;
    caixa_mem[total_caixa_mem++]=l; return l.id;
}
int pers_memoria_carregar_caixa(CaixaLancamento *lista, int max){
    int n = (total_caixa_mem < max) ? total_caixa_mem : max;
    for(int i=0;i<n;i++) lista[i]=caixa_mem[i];
    return n;
}

int pers_memoria_salvar_conta_receber(ContaReceber c){
    for(int i=0;i<total_contas_receber_mem;i++){ if(contas_receber_mem[i].id==c.id){ contas_receber_mem[i]=c; return c.id; } }
    if(c.id<=0){ int max_id=0; for(int i=0;i<total_contas_receber_mem;i++) if(contas_receber_mem[i].id>max_id) max_id=contas_receber_mem[i].id; c.id=max_id+1; }
    if(total_contas_receber_mem >= MEM_MAX_CONTAS_RECEBER) return 0;
    contas_receber_mem[total_contas_receber_mem++]=c; return c.id;
}
int pers_memoria_carregar_contas_receber(ContaReceber *lista, int max){
    int n = (total_contas_receber_mem < max) ? total_contas_receber_mem : max;
    for(int i=0;i<n;i++) lista[i]=contas_receber_mem[i];
    return n;
}

int pers_memoria_salvar_compra(Compra c){
    for(int i=0;i<total_compras_mem;i++){ if(compras_mem[i].id==c.id){ compras_mem[i]=c; return c.id; } }
    if(c.id<=0){ int max_id=0; for(int i=0;i<total_compras_mem;i++) if(compras_mem[i].id>max_id) max_id=compras_mem[i].id; c.id=max_id+1; }
    if(total_compras_mem >= MEM_MAX_COMPRAS) return 0;
    compras_mem[total_compras_mem++]=c; return c.id;
}
int pers_memoria_carregar_compras(Compra *lista, int max){
    int n = (total_compras_mem < max) ? total_compras_mem : max;
    for(int i=0;i<n;i++) lista[i]=compras_mem[i];
    return n;
}

int pers_memoria_salvar_compra_item(CompraItem it){
    for(int i=0;i<total_compra_itens_mem;i++){ if(compra_itens_mem[i].id==it.id && it.id>0){ compra_itens_mem[i]=it; return it.id; } }
    if(it.id<=0){ int max_id=0; for(int i=0;i<total_compra_itens_mem;i++) if(compra_itens_mem[i].id>max_id) max_id=compra_itens_mem[i].id; it.id=max_id+1; }
    if(total_compra_itens_mem >= MEM_MAX_COMPRA_ITENS) return 0;
    compra_itens_mem[total_compra_itens_mem++]=it; return it.id;
}
int pers_memoria_carregar_compra_itens_por_compra(int compra_id, CompraItem *lista, int max){
    int n=0; for(int i=0;i<total_compra_itens_mem && n<max;i++){ if(compra_itens_mem[i].compra_id==compra_id){ lista[n++]=compra_itens_mem[i]; } }
    return n;
}

int pers_memoria_salvar_conta_pagar(ContaPagar c){
    for(int i=0;i<total_contas_pagar_mem;i++){ if(contas_pagar_mem[i].id==c.id){ contas_pagar_mem[i]=c; return c.id; } }
    if(c.id<=0){ int max_id=0; for(int i=0;i<total_contas_pagar_mem;i++) if(contas_pagar_mem[i].id>max_id) max_id=contas_pagar_mem[i].id; c.id=max_id+1; }
    if(total_contas_pagar_mem >= MEM_MAX_CONTAS_PAGAR) return 0;
    contas_pagar_mem[total_contas_pagar_mem++]=c; return c.id;
}
int pers_memoria_carregar_contas_pagar(ContaPagar *lista, int max){
    int n = (total_contas_pagar_mem < max) ? total_contas_pagar_mem : max;
    for(int i=0;i<n;i++) lista[i]=contas_pagar_mem[i];
    return n;
}