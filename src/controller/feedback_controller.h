#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#include <stdio.h>
#include "../model/cliente.h"
#include "../model/evento.h"
#include "../model/recurso.h"
#include "../model/transacoes.h"

/* ========================================================================
 * TIPOS E ESTRUTURAS DE FILTRO
 * ======================================================================== */

typedef struct {
    int codigo_min;
    int codigo_max;
    int ordem_alfabetica; // 0=por código, 1=por nome
} FiltroCliente;

typedef struct {
    int cliente_id;  // -1 = todos
    char data_min[11];  // YYYY-MM-DD
    char data_max[11];  // YYYY-MM-DD
    int status;  // -1=todos, 1=Orçamento, 2=Aprovado, 3=Finalizado
} FiltroEvento;

typedef struct {
    int recurso_id;  // -1 = todos
    char data_min[11];  // YYYY-MM-DD
    char data_max[11];  // YYYY-MM-DD
} FiltroRecurso;

typedef struct {
    int codigo_min;
    int codigo_max;
    char categoria[60];  // vazio = todos
} FiltroEquipamento;

typedef struct {
    int cliente_id;  // -1 = todos
    char data_min[11];  // data de vencimento min
    char data_max[11];  // data de vencimento max
} FiltroContasReceber;

typedef struct {
    int fornecedor_id;  // -1 = todos
    char data_min[11];  // data de vencimento min
    char data_max[11];  // data de vencimento max
} FiltroContasPagar;

typedef struct {
    char data_min[11];
    char data_max[11];
} FiltroCaixa;

/* ========================================================================
 * CLIENTES
 * ======================================================================== */

// Gera lista de clientes com filtros
int feedback_relatorio_clientes(Cliente *buffer, int max, FiltroCliente filtro);

// Exporta relatório de clientes para CSV
int feedback_exportar_clientes_csv(const char *caminho, FiltroCliente filtro);

/* ========================================================================
 *  EVENTOS
 * ======================================================================== */

// Gera lista de eventos com filtros
int feedback_relatorio_eventos(Evento *buffer, int max, FiltroEvento filtro);

// Exporta relatório de eventos para CSV
int feedback_exportar_eventos_csv(const char *caminho, FiltroEvento filtro);

// Obtém nome do cliente para exibição
void feedback_obter_cliente_nome(int cliente_id, char *buffer, int max);

/* ========================================================================
 *  CRONOGRAMA DE RECURSOS
 * ======================================================================== */

typedef struct {
    int recurso_id;
    char recurso_desc[120];
    int evento_id;
    char evento_nome[120];
    char data_inicio[11];
    char data_fim[11];
} AlocacaoRecurso;

// Gera cronograma de alocação de recursos
int feedback_cronograma_alocacao(AlocacaoRecurso *buffer, int max, FiltroRecurso filtro);

// Exporta cronograma para CSV
int feedback_exportar_cronograma_csv(const char *caminho, FiltroRecurso filtro);

/* ========================================================================
 *  EQUIPAMENTOS
 * ======================================================================== */

// Gera lista de equipamentos com filtros
int feedback_relatorio_equipamentos(Recurso *buffer, int max, FiltroEquipamento filtro);

// Exporta relatório de equipamentos para CSV
int feedback_exportar_equipamentos_csv(const char *caminho, FiltroEquipamento filtro);

/* ========================================================================
 * CONTAS A RECEBER
 * ======================================================================== */

// Gera lista de contas a receber com filtros
int feedback_relatorio_contas_receber(ContaReceber *buffer, int max, FiltroContasReceber filtro);

// Exporta relatório para CSV
int feedback_exportar_contas_receber_csv(const char *caminho, FiltroContasReceber filtro);

/* ========================================================================
 * CONTAS A PAGAR
 * ======================================================================== */

// Gera lista de contas a pagar com filtros
int feedback_relatorio_contas_pagar(ContaPagar *buffer, int max, FiltroContasPagar filtro);

// Exporta relatório para CSV
int feedback_exportar_contas_pagar_csv(const char *caminho, FiltroContasPagar filtro);

// Obtém nome do fornecedor para exibição
void feedback_obter_fornecedor_nome(int fornecedor_id, char *buffer, int max);

/* ========================================================================
 *  MOVIMENTAÇÃO DE CAIXA
 * ======================================================================== */

// Gera lista de lançamentos de caixa com filtro de período
int feedback_relatorio_caixa(CaixaLancamento *buffer, int max, FiltroCaixa filtro);

// Exporta relatório para CSV
int feedback_exportar_caixa_csv(const char *caminho, FiltroCaixa filtro);

/* ========================================================================
 * UTILITÁRIOS
 * ======================================================================== */

// Compara datas no formato YYYY-MM-DD: retorna -1 se data1<data2, 0 se igual, 1 se data1>data2
int feedback_comparar_datas(const char *data1, const char *data2);

// Verifica se data está dentro de um intervalo
int feedback_data_no_intervalo(const char *data, const char *min, const char *max);

#endif // FEEDBACK_CONTROLLER_H
