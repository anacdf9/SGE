#ifndef PERS_BINARIO_H
#define PERS_BINARIO_H

#include "cliente.h"
#include "recurso.h"
#include "equipe.h"
#include "fornecedor.h"
#include "operador.h"
#include "produtora.h"
#include "evento.h"

void pers_binario_inicializar(void);
void pers_binario_finalizar(void);

int pers_binario_salvar_cliente(Cliente c);
int pers_binario_carregar_clientes(Cliente *lista, int max);
int pers_binario_remover_cliente(int id);

int pers_binario_salvar_recurso(Recurso r);
int pers_binario_carregar_recursos(Recurso *lista, int max);
int pers_binario_remover_recurso(int id);

int pers_binario_salvar_equipe(Equipe e);
int pers_binario_carregar_equipes(Equipe *lista, int max);
int pers_binario_remover_equipe(int id);

int pers_binario_salvar_fornecedor(Fornecedor f);
int pers_binario_carregar_fornecedores(Fornecedor *lista, int max);
int pers_binario_remover_fornecedor(int id);

int pers_binario_salvar_operador(Operador o);
int pers_binario_carregar_operadores(Operador *lista, int max);
int pers_binario_remover_operador(int id);

int pers_binario_salvar_produtora(Produtora p);
int pers_binario_obter_produtora(Produtora *out);
int pers_binario_remover_produtora(void);

/* Evento */
int pers_binario_salvar_evento(Evento e);
int pers_binario_carregar_eventos(Evento *lista, int max);
int pers_binario_remover_evento(int id);

#endif // PERS_BINARIO_H