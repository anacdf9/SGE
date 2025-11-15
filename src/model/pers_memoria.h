#ifndef PERS_MEMORIA_H
#define PERS_MEMORIA_H

#include "cliente.h"
#include "recurso.h"
#include "equipe.h"
#include "fornecedor.h"
#include "operador.h"
#include "produtora.h"

void pers_memoria_inicializar(void);
void pers_memoria_finalizar(void);

int pers_memoria_salvar_cliente(Cliente c);
int pers_memoria_carregar_clientes(Cliente *lista, int max);
int pers_memoria_remover_cliente(int id);

/* Recurso */
int pers_memoria_salvar_recurso(Recurso r);
int pers_memoria_carregar_recursos(Recurso *lista, int max);
int pers_memoria_remover_recurso(int id);

/* Equipe */
int pers_memoria_salvar_equipe(Equipe e);
int pers_memoria_carregar_equipes(Equipe *lista, int max);
int pers_memoria_remover_equipe(int id);

/* Fornecedor */
int pers_memoria_salvar_fornecedor(Fornecedor f);
int pers_memoria_carregar_fornecedores(Fornecedor *lista, int max);
int pers_memoria_remover_fornecedor(int id);

/* Operador */
int pers_memoria_salvar_operador(Operador o);
int pers_memoria_carregar_operadores(Operador *lista, int max);
int pers_memoria_remover_operador(int id);

/* Produtora (cadastro único) */
int pers_memoria_salvar_produtora(Produtora p);
int pers_memoria_obter_produtora(Produtora *out);
int pers_memoria_remover_produtora(void);

#endif // PERS_MEMORIA_H