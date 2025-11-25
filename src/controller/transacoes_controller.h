#ifndef TRANSACOES_CONTROLLER_H
#define TRANSACOES_CONTROLLER_H

#include "../model/transacoes.h"

int caixa_lancar(CaixaLancamento l);
int caixa_listar(CaixaLancamento *buf, int max);

double caixa_saldo_atual(void);

int conta_receber_criar(int evento_id, double valor_fatura, const char *data_emissao, const char *data_vencimento);
int conta_receber_listar(ContaReceber *buf, int max);
int conta_receber_baixa(int id, double valor, const char *forma_pagamento);

int compra_registrar(Compra c, CompraItem *itens, int n_itens, int parcelas);
int compra_listar(Compra *buf, int max);
int compra_itens_por_compra(int compra_id, CompraItem *buf, int max);
int compra_reprocessar_locacoes(void);

int contas_pagar_listar(ContaPagar *buf, int max);
int contas_pagar_baixar(int id);

#endif
