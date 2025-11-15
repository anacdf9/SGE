#include <string.h>
#include "pers_memoria.h"

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

static Produtora produtora_mem; /* cadastro único */
static int produtora_definida = 0;

void pers_memoria_inicializar(void) {
    total_clientes_mem = 0;
    total_recursos_mem = 0;
    total_equipes_mem = 0;
    total_fornecedores_mem = 0;
    total_operadores_mem = 0;
    produtora_definida = 0;
}

void pers_memoria_finalizar(void) {
    /* nada específico: memória será liberada ao terminar o programa */
    total_clientes_mem = 0;
    total_recursos_mem = 0;
    total_equipes_mem = 0;
    total_fornecedores_mem = 0;
    total_operadores_mem = 0;
    produtora_definida = 0;
}

int pers_memoria_salvar_cliente(Cliente c) {
    /* se id já existe -> atualizar */
    for (int i = 0; i < total_clientes_mem; i++) {
        if (clientes_mem[i].id == c.id) {
            // clients_mem_copy:  <-- TIREI ESSE LABEL QUE DAVA WARNING
            clientes_mem[i] = c;
            return 1;
        }
    }
    /* inserir novo: verifica espaço e unicidade do id */
    if (total_clientes_mem >= MEM_MAX_CLIENTES) return 0;
    /* evita duplicar id já existente (verificação acima cobre) */
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