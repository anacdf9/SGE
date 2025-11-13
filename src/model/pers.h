#ifndef PERS_H
#define PERS_H

#include "cliente.h"
#include "recurso.h"
#include "equipe.h"
#include "fornecedor.h"
#include "operador.h"
#include "produtora.h"
#include "config.h"
#include <stdlib.h>   

/* Inicializa o sistema de persistência (usa TIPO_PERSISTENCIA por padrão) */
void pers_inicializar(TipoPersistencia tipo);
void pers_finalizar(void);

/* CRUD cliente (fachada) */
int pers_salvar_cliente(Cliente c);
int pers_carregar_clientes(Cliente *lista, int max);
int pers_remover_cliente(int id);

/* CRUD recurso */
int pers_salvar_recurso(Recurso r);
int pers_carregar_recursos(Recurso *lista, int max);
int pers_remover_recurso(int id);

/* CRUD equipe */
int pers_salvar_equipe(Equipe e);
int pers_carregar_equipes(Equipe *lista, int max);
int pers_remover_equipe(int id);

/* CRUD fornecedor */
int pers_salvar_fornecedor(Fornecedor f);
int pers_carregar_fornecedores(Fornecedor *lista, int max);
int pers_remover_fornecedor(int id);

/* CRUD operador */
int pers_salvar_operador(Operador o);
int pers_carregar_operadores(Operador *lista, int max);
int pers_remover_operador(int id);

/* Produtora (único) */
int pers_salvar_produtora(Produtora p);
int pers_obter_produtora(Produtora *out);

#endif // PERS_H