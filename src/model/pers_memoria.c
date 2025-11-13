#include <string.h>
#include "pers_memoria.h"

#define MEM_MAX_CLIENTES 100

static Cliente clientes_mem[MEM_MAX_CLIENTES];
static int total_clientes_mem = 0;

void pers_memoria_inicializar(void) {
    total_clientes_mem = 0;
}

void pers_memoria_finalizar(void) {
    /* nada específico: memória será liberada ao terminar o programa */
    total_clientes_mem = 0;
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