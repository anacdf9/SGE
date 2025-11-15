#ifndef PERS_TEXTO_H
#define PERS_TEXTO_H

#include "cliente.h"
#include "recurso.h"
#include "equipe.h"
#include "fornecedor.h"
#include "operador.h"
#include "produtora.h"

void pers_texto_inicializar(void);
void pers_texto_finalizar(void);

int pers_texto_salvar_cliente(Cliente c);
int pers_texto_carregar_clientes(Cliente *lista, int max);
int pers_texto_remover_cliente(int id);

int pers_texto_salvar_recurso(Recurso r);
int pers_texto_carregar_recursos(Recurso *lista, int max);
int pers_texto_remover_recurso(int id);

int pers_texto_salvar_equipe(Equipe e);
int pers_texto_carregar_equipes(Equipe *lista, int max);
int pers_texto_remover_equipe(int id);

int pers_texto_salvar_fornecedor(Fornecedor f);
int pers_texto_carregar_fornecedores(Fornecedor *lista, int max);
int pers_texto_remover_fornecedor(int id);

int pers_texto_salvar_operador(Operador o);
int pers_texto_carregar_operadores(Operador *lista, int max);
int pers_texto_remover_operador(int id);

int pers_texto_salvar_produtora(Produtora p);
int pers_texto_obter_produtora(Produtora *out);
int pers_texto_remover_produtora(void);

#endif // PERS_TEXTO_H