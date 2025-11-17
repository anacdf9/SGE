#include <stdio.h>
#include "pers.h"

/* includes dos backends */
#include "pers_memoria.h"
#include "pers_texto.h"
#include "pers_binario.h"
#include "evento.h"

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

/* ===== Recurso ===== */
int pers_salvar_recurso(Recurso r) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_salvar_recurso(r);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_salvar_recurso(r);
    if (tipo_atual == PERS_BINARIO) return pers_binario_salvar_recurso(r);
    return 0;
}

int pers_carregar_recursos(Recurso *lista, int max) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_carregar_recursos(lista, max);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_carregar_recursos(lista, max);
    if (tipo_atual == PERS_BINARIO) return pers_binario_carregar_recursos(lista, max);
    return 0;
}

int pers_remover_recurso(int id) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_remover_recurso(id);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_remover_recurso(id);
    if (tipo_atual == PERS_BINARIO) return pers_binario_remover_recurso(id);
    return 0;
}

/* ===== Equipe ===== */
int pers_salvar_equipe(Equipe e) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_salvar_equipe(e);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_salvar_equipe(e);
    if (tipo_atual == PERS_BINARIO) return pers_binario_salvar_equipe(e);
    return 0;
}

int pers_carregar_equipes(Equipe *lista, int max) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_carregar_equipes(lista, max);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_carregar_equipes(lista, max);
    if (tipo_atual == PERS_BINARIO) return pers_binario_carregar_equipes(lista, max);
    return 0;
}

int pers_remover_equipe(int id) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_remover_equipe(id);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_remover_equipe(id);
    if (tipo_atual == PERS_BINARIO) return pers_binario_remover_equipe(id);
    return 0;
}

/* ===== Fornecedor ===== */
int pers_salvar_fornecedor(Fornecedor f) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_salvar_fornecedor(f);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_salvar_fornecedor(f);
    if (tipo_atual == PERS_BINARIO) return pers_binario_salvar_fornecedor(f);
    return 0;
}

int pers_carregar_fornecedores(Fornecedor *lista, int max) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_carregar_fornecedores(lista, max);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_carregar_fornecedores(lista, max);
    if (tipo_atual == PERS_BINARIO) return pers_binario_carregar_fornecedores(lista, max);
    return 0;
}

int pers_remover_fornecedor(int id) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_remover_fornecedor(id);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_remover_fornecedor(id);
    if (tipo_atual == PERS_BINARIO) return pers_binario_remover_fornecedor(id);
    return 0;
}

/* ===== Operador ===== */
int pers_salvar_operador(Operador o) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_salvar_operador(o);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_salvar_operador(o);
    if (tipo_atual == PERS_BINARIO) return pers_binario_salvar_operador(o);
    return 0;
}

int pers_carregar_operadores(Operador *lista, int max) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_carregar_operadores(lista, max);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_carregar_operadores(lista, max);
    if (tipo_atual == PERS_BINARIO) return pers_binario_carregar_operadores(lista, max);
    return 0;
}

int pers_remover_operador(int id) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_remover_operador(id);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_remover_operador(id);
    if (tipo_atual == PERS_BINARIO) return pers_binario_remover_operador(id);
    return 0;
}

/* ===== Produtora (único) ===== */
int pers_salvar_produtora(Produtora p) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_salvar_produtora(p);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_salvar_produtora(p);
    if (tipo_atual == PERS_BINARIO) return pers_binario_salvar_produtora(p);
    return 0;
}

int pers_obter_produtora(Produtora *out) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_obter_produtora(out);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_obter_produtora(out);
    if (tipo_atual == PERS_BINARIO) return pers_binario_obter_produtora(out);
    return 0;
}

int pers_remover_produtora(void) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_remover_produtora();
    if (tipo_atual == PERS_TEXTO)   return pers_texto_remover_produtora();
    if (tipo_atual == PERS_BINARIO) return pers_binario_remover_produtora();
    return 0;
}

/* ===== Evento ===== */
int pers_salvar_evento(Evento e) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_salvar_evento(e);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_salvar_evento(e);
    if (tipo_atual == PERS_BINARIO) return pers_binario_salvar_evento(e);
    return 0;
}

int pers_carregar_eventos(Evento *lista, int max) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_carregar_eventos(lista, max);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_carregar_eventos(lista, max);
    if (tipo_atual == PERS_BINARIO) return pers_binario_carregar_eventos(lista, max);
    return 0;
}

int pers_remover_evento(int id) {
    if (tipo_atual == PERS_MEMORIA) return pers_memoria_remover_evento(id);
    if (tipo_atual == PERS_TEXTO)   return pers_texto_remover_evento(id);
    if (tipo_atual == PERS_BINARIO) return pers_binario_remover_evento(id);
    return 0;
}