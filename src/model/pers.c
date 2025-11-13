#include <stdio.h>
#include "pers.h"

/* includes dos backends */
#include "pers_memoria.h"
#include "pers_texto.h"
#include "pers_binario.h"

static TipoPersistencia tipo_atual = TIPO_PERSISTENCIA;

void pers_inicializar(TipoPersistencia tipo) {
    tipo_atual = tipo;
    if (tipo_atual == PERS_MEMORIA) {
        pers_memoria_inicializar();
    } else if (tipo_atual == PERS_TEXTO) {
        pers_texto_inicializar();
    } else if (tipo_atual == PERS_BINARIO) {
        pers_binario_inicializar();
    }
}

void pers_finalizar(void) {
    if (tipo_atual == PERS_MEMORIA) {
        pers_memoria_finalizar();
    } else if (tipo_atual == PERS_TEXTO) {
        pers_texto_finalizar();
    } else if (tipo_atual == PERS_BINARIO) {
        pers_binario_finalizar();
    }
}

/* operações cliente roteadas */
int pers_salvar_cliente(Cliente c) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_salvar_cliente(c);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_salvar_cliente(c);
    if (tipo_atual == PERS_BINARIO) return pers_binario_salvar_cliente(c);
    return 0;
}

int pers_carregar_clientes(Cliente *lista, int max) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_carregar_clientes(lista, max);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_carregar_clientes(lista, max);
    if (tipo_atual == PERS_BINARIO) return pers_binario_carregar_clientes(lista, max);
    return 0;
}

int pers_remover_cliente(int id) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_remover_cliente(id);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_remover_cliente(id);
    if (tipo_atual == PERS_BINARIO) return pers_binario_remover_cliente(id);
    return 0;
}