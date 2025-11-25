#ifndef TRANSACOES_H
#define TRANSACOES_H

#include "evento.h"
#include "recurso.h"
#include "fornecedor.h"

typedef struct {
    int id;
    char data[11]; // YYYY-MM-DD
    char tipo[16]; // ENTRADA / SAIDA
    char forma[16]; // DINHEIRO / DEBITO / CREDITO
    double valor;
    char descricao[128];
} CaixaLancamento;

typedef struct {
    int id;
    int evento_id;
    double valor_fatura; // valor total a receber
    double valor_recebido; // acumulado
    char data_emissao[11];
    char data_vencimento[11];
    int status; // 0=Aberto 1=Parcial 2=Liquidado
} ContaReceber;

typedef struct {
    int id;
    int compra_id; // FK
    int recurso_id; // produto comprado (recurso)
    double preco_custo_unit; // custo unitário informado
    int quantidade;
    double total_item; // preco_custo_unit * quantidade
    double preco_locacao_unit; // calculado: custo + frete_unit + imposto_unit + margem*custo
} CompraItem;

typedef struct {
    int id;
    int fornecedor_id;
    char data[11];
    double frete;
    double imposto;
    double total_produtos; // soma dos itens
    double total_nota; // total_produtos + frete + imposto
    int pagamento_tipo; // 0=Avista 1=Prazo
    double entrada; // se prazo pode ter entrada
} Compra;

typedef struct {
    int id;
    int fornecedor_id;
    int compra_id; // origem
    char data_vencimento[11];
    double valor;
    int status; // 0=Aberto 1=Pago
} ContaPagar;

#endif
