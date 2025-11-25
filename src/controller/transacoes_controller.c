/*
 * ========================================================================
 * CONTROLADOR: Transações Financeiras
 * Descrição: Lógica de negócio para gerenciar operações financeiras
 *           (Caixa, Contas a Receber, Compras, Contas a Pagar)
 * ========================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "transacoes_controller.h"
#include "cliente_controller.h"
#include "recurso_controller.h"
#include "fornecedor_controller.h"

#include "../model/pers.h"
#include "../model/transacoes.h"
#include "../model/recurso.h"
#include "../model/cliente.h"
#include "../model/fornecedor.h"
#include "../model/evento.h"

/* ========================================================================
 * FUNÇÕES AUXILIARES
 * ======================================================================== */

/*
 * Função: parse_date_ok
 * Descrição: Valida se uma data está no formato correto (10 caracteres)
 */
static int parse_date_ok(const char *d) {
    return d && strlen(d) == 10;
}

/*
 * Função: arred
 * Descrição: Arredonda um valor para 2 casas decimais
 * Método: Multiplica por 100, arredonda e divide por 100
 */
static double arred(double v) {
    return ((long long)(v * 100 + (v >= 0 ? 0.5 : -0.5))) / 100.0;
}

/* ========================================================================
 * FUNÇÕES DO CAIXA
 * ======================================================================== */

/*
 * Função: caixa_lancar
 * Descrição: Registra um novo lançamento no caixa (entrada ou saída)
 * Retorno: 1 se sucesso, 0 se falha
 */
int caixa_lancar(CaixaLancamento l) {
    /* Validar data e valor */
    if (!parse_date_ok(l.data) || l.valor == 0) {
        return 0;
    }
    
    /* Salvar no banco de dados */
    return pers_salvar_caixa(l);
}

/*
 * Função: caixa_listar
 * Descrição: Lista todos os lançamentos do caixa
 */
int caixa_listar(CaixaLancamento *buf, int max) {
    return pers_carregar_caixa(buf, max);
}

/*
 * Função: caixa_saldo_atual
 * Descrição: Calcula o saldo atual do caixa
 *            (soma entradas - soma saídas)
 */
double caixa_saldo_atual(void) {
    CaixaLancamento arr[1024];
    int n = pers_carregar_caixa(arr, 1024);
    double s = 0;
    
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].tipo, "ENTRADA") == 0) {
            s += arr[i].valor;
        } else {
            s -= arr[i].valor;
        }
    }
    
    return s;
}

int conta_receber_criar(int evento_id, double valor_fatura, const char *data_emissao, const char *data_vencimento){
    if(evento_id<=0 || valor_fatura<=0 || !parse_date_ok(data_emissao) || !parse_date_ok(data_vencimento)) return 0;
    ContaReceber cr; memset(&cr,0,sizeof(cr));
    cr.evento_id = evento_id; cr.valor_fatura = valor_fatura; cr.valor_recebido = 0; strncpy(cr.data_emissao,data_emissao,10); strncpy(cr.data_vencimento,data_vencimento,10); cr.status=0;
    return pers_salvar_conta_receber(cr);
}

int conta_receber_listar(ContaReceber *buf, int max){ return pers_carregar_contas_receber(buf,max); }

int conta_receber_baixa(int id, double valor, const char *forma_pagamento){
    if(id<=0 || valor<=0) return 0;
    ContaReceber arr[1024]; int n = pers_carregar_contas_receber(arr,1024); int idx=-1; for(int i=0;i<n;i++){ if(arr[i].id==id){ idx=i; break; } }
    if(idx<0) return 0;
    double restante = arr[idx].valor_fatura - arr[idx].valor_recebido; if(valor>restante) valor = restante;
    arr[idx].valor_recebido += valor;
    if(arr[idx].valor_recebido >= arr[idx].valor_fatura) arr[idx].status = 2; else arr[idx].status = 1;
    CaixaLancamento l; memset(&l,0,sizeof(l));
    strcpy(l.tipo,"ENTRADA"); strncpy(l.forma, forma_pagamento?forma_pagamento:"DINHEIRO", 15);
    strncpy(l.data, arr[idx].data_emissao, 10); snprintf(l.descricao, sizeof(l.descricao), "Recebimento Evento %d", arr[idx].evento_id);
    pers_salvar_caixa(l);
    for(int i=0;i<n;i++) pers_salvar_conta_receber(arr[i]);
    return 1;
}

int compra_registrar(Compra c, CompraItem *itens, int n_itens, int parcelas){
    if(c.fornecedor_id<=0 || !parse_date_ok(c.data) || n_itens<=0) return 0;
    if(!itens) return 0;
    int soma_qtd=0; double soma_prod=0; for(int i=0;i<n_itens;i++){ soma_qtd+=itens[i].quantidade; soma_prod += itens[i].preco_custo_unit * itens[i].quantidade; }
    c.total_produtos = soma_prod; c.total_nota = soma_prod + c.frete + c.imposto;
    int compra_id = pers_salvar_compra(c); if(compra_id<=0) return 0;
    double frete_unit = soma_qtd>0 ? c.frete / soma_qtd : 0; double imposto_unit = soma_qtd>0 ? c.imposto / soma_qtd : 0;
    Produtora prod; double margem=0.10; if(pers_obter_produtora(&prod)) margem = prod.margem_lucro_padrao;
    for(int i=0;i<n_itens;i++){
        itens[i].compra_id = compra_id; itens[i].total_item = itens[i].preco_custo_unit * itens[i].quantidade;
        double aluguel = itens[i].preco_custo_unit + frete_unit + imposto_unit + (margem * itens[i].preco_custo_unit);
        itens[i].preco_locacao_unit = arred(aluguel);
        pers_salvar_compra_item(itens[i]);
        Recurso r; if(recurso_obter(itens[i].recurso_id,&r)){
            int novo_qtd = r.quantidade + itens[i].quantidade;
            double novo_total_custo = r.preco_custo * r.quantidade + itens[i].preco_custo_unit * itens[i].quantidade;
            double novo_custo_medio = novo_qtd>0 ? novo_total_custo / novo_qtd : r.preco_custo;
            r.quantidade = novo_qtd; r.preco_custo = arred(novo_custo_medio);
            r.valor_locacao = itens[i].preco_locacao_unit;
            recurso_salvar(r);
        }
    }
    if(c.pagamento_tipo==0){
        CaixaLancamento l; memset(&l,0,sizeof(l));
        strcpy(l.tipo,"SAIDA"); strcpy(l.forma,"DINHEIRO");
        l.valor=c.total_nota; strncpy(l.data,c.data,10);
        strcpy(l.descricao,"Compra avista");
        pers_salvar_caixa(l);
    } else {
        if(c.entrada>0){
            CaixaLancamento l; memset(&l,0,sizeof(l));
            strcpy(l.tipo,"SAIDA"); strcpy(l.forma,"DINHEIRO");
            l.valor=c.entrada; strncpy(l.data,c.data,10);
            strcpy(l.descricao,"Entrada compra");
            pers_salvar_caixa(l);
        }
        double restante = c.total_nota - c.entrada;
        if(restante<0) restante=0;
        if(parcelas<1) parcelas=1;
        double parcela_val = arred(restante / parcelas);
        for(int p=0;p<parcelas;p++){
            ContaPagar cp; memset(&cp,0,sizeof(cp));
            cp.fornecedor_id = c.fornecedor_id;
            cp.compra_id = compra_id;
            strncpy(cp.data_vencimento,c.data,10);
            cp.valor = parcela_val;
            cp.status=0;
            pers_salvar_conta_pagar(cp);
        }
    }
    return compra_id;
}

int compra_listar(Compra *buf, int max){ return pers_carregar_compras(buf,max); }
int compra_itens_por_compra(int compra_id, CompraItem *buf, int max){ return pers_carregar_compra_itens_por_compra(compra_id,buf,max); }

int compra_reprocessar_locacoes(void){
    Compra compras[1024]; int nc = compra_listar(compras,1024);
    Produtora prod; double margem=0.10; if(pers_obter_produtora(&prod)) margem = prod.margem_lucro_padrao;
    int atualizados=0;
    for(int i=0;i<nc;i++){
        CompraItem itens[1024]; int ni = compra_itens_por_compra(compras[i].id, itens, 1024);
        if(ni<=0) continue;
        int soma_qtd=0; for(int k=0;k<ni;k++) soma_qtd += itens[k].quantidade;
        double frete_unit = soma_qtd>0 ? compras[i].frete / soma_qtd : 0;
        double imposto_unit = soma_qtd>0 ? compras[i].imposto / soma_qtd : 0;
        for(int k=0;k<ni;k++){
            if(itens[k].preco_locacao_unit<=0){
                double aluguel = itens[k].preco_custo_unit + frete_unit + imposto_unit + (margem * itens[k].preco_custo_unit);
                itens[k].preco_locacao_unit = arred(aluguel);
                pers_salvar_compra_item(itens[k]);
                atualizados++;
            }
        }
    }
    return atualizados;
}

int contas_pagar_listar(ContaPagar *buf, int max){ return pers_carregar_contas_pagar(buf,max); }

int contas_pagar_baixar(int id){
    ContaPagar arr[1024];
    int n = pers_carregar_contas_pagar(arr,1024);
    int idx=-1;
    for(int i=0;i<n;i++){ if(arr[i].id==id){ idx=i; break; } }
    if(idx<0) return 0;
    if(arr[idx].status==1) return 0;
    CaixaLancamento l; memset(&l,0,sizeof(l));
    strcpy(l.tipo,"SAIDA"); strcpy(l.forma,"DINHEIRO");
    l.valor = arr[idx].valor;
    strncpy(l.data, arr[idx].data_vencimento,10);
    snprintf(l.descricao,sizeof(l.descricao),"Pg compra %d", arr[idx].compra_id);
    pers_salvar_caixa(l);
    arr[idx].status=1;
    for(int i=0;i<n;i++) pers_salvar_conta_pagar(arr[i]);
    return 1;
}
