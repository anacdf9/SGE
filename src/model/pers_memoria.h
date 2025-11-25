#ifndef PERS_MEMORIA_H
#define PERS_MEMORIA_H

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

/* Evento */
int pers_memoria_salvar_evento(Evento e);
int pers_memoria_carregar_eventos(Evento *lista, int max);
int pers_memoria_remover_evento(int id);

/* Itens de evento (modo memória) */
int pers_memoria_salvar_evento_item(EventoItem it);
int pers_memoria_carregar_evento_itens(EventoItem *lista, int max);
int pers_memoria_remover_evento_itens_por_evento(int evento_id);

/* Itens de equipe por evento (modo memória) */
int pers_memoria_salvar_evento_equipe(EventoEquipe ee);
int pers_memoria_carregar_evento_equipes(EventoEquipe *lista, int max);
int pers_memoria_remover_evento_equipes_por_evento(int evento_id);

/* Itens de fornecedor por evento (modo memória) */
int pers_memoria_salvar_evento_fornecedor(EventoFornecedor ef);
int pers_memoria_carregar_evento_fornecedores(EventoFornecedor *lista, int max);
int pers_memoria_remover_evento_fornecedores_por_evento(int evento_id);

/* ==================== Transações ==================== */
int pers_memoria_salvar_caixa(CaixaLancamento l);
int pers_memoria_carregar_caixa(CaixaLancamento *lista, int max);

int pers_memoria_salvar_conta_receber(ContaReceber c);
int pers_memoria_carregar_contas_receber(ContaReceber *lista, int max);

int pers_memoria_salvar_compra(Compra c);
int pers_memoria_carregar_compras(Compra *lista, int max);

int pers_memoria_salvar_compra_item(CompraItem it);
int pers_memoria_carregar_compra_itens_por_compra(int compra_id, CompraItem *lista, int max);

int pers_memoria_salvar_conta_pagar(ContaPagar c);
int pers_memoria_carregar_contas_pagar(ContaPagar *lista, int max);

#endif // PERS_MEMORIA_H