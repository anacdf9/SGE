#ifndef PERS_TEXTO_H
#define PERS_TEXTO_H

#include "cliente.h"
#include "recurso.h"
#include "equipe.h"
#include "fornecedor.h"
#include "operador.h"
#include "produtora.h"
#include "evento.h"
#include "evento_item.h"
#include "evento_equipe.h"
#include "evento_fornecedor.h"
#include "transacoes.h"

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

/* Evento */
int pers_texto_salvar_evento(Evento e);
int pers_texto_carregar_eventos(Evento *lista, int max);
int pers_texto_remover_evento(int id);

/* Itens de evento (arquivo texto) */
int pers_texto_salvar_evento_item(EventoItem it);
int pers_texto_carregar_evento_itens(EventoItem *lista, int max);
int pers_texto_remover_evento_itens_por_evento(int evento_id);

/* Equipes de evento (arquivo texto) */
int pers_texto_salvar_evento_equipe(EventoEquipe ee);
int pers_texto_carregar_evento_equipes(EventoEquipe *lista, int max);
int pers_texto_remover_evento_equipes_por_evento(int evento_id);

/* Fornecedores de evento (arquivo texto) */
int pers_texto_salvar_evento_fornecedor(EventoFornecedor ef);
int pers_texto_carregar_evento_fornecedores(EventoFornecedor *lista, int max);
int pers_texto_remover_evento_fornecedores_por_evento(int evento_id);

/* Caixa */
int pers_texto_salvar_caixa(CaixaLancamento l);
int pers_texto_carregar_caixa(CaixaLancamento *lista, int max);

/* Contas a Receber */
int pers_texto_salvar_conta_receber(ContaReceber c);
int pers_texto_carregar_contas_receber(ContaReceber *lista,int max);

/* Compras */
int pers_texto_salvar_compra(Compra c);
int pers_texto_carregar_compras(Compra *lista,int max);
int pers_texto_salvar_compra_item(CompraItem it);
int pers_texto_carregar_compra_itens_por_compra(int compra_id, CompraItem *lista,int max);

/* Contas a Pagar */
int pers_texto_salvar_conta_pagar(ContaPagar c);
int pers_texto_carregar_contas_pagar(ContaPagar *lista,int max);


#endif // PERS_TEXTO_H