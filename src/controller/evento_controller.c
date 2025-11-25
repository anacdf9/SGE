/*
===============================================================================
   EVENTO CONTROLLER
   
   Responsável por:
   - Gerenciar criação de orçamentos e eventos
   - Controlar alocação de recursos, equipe e fornecedores
   - Calcular valores totais automaticamente
   - Verificar conflitos de agenda
   - Aprovar e finalizar eventos
   
   Status de um Evento:
   - ORÇAMENTO: proposta inicial (pode ser editada)
   - APROVADO: confirmado (recursos reservados)
   - FINALIZADO: concluído (valor final consolidado)
===============================================================================
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "evento_controller.h"
#include "recurso_controller.h"
#include "equipe_controller.h"
#include "../model/pers.h"
#include "../model/recurso.h"
#include "../model/equipe.h"
#include "../model/fornecedor.h"
#include "../model/evento_item.h"
#include "../model/evento_equipe.h"
#include "../model/evento_fornecedor.h"


/* ========================================
   FUNÇÕES AUXILIARES
   ======================================== */

// Converte data "YYYY-MM-DD" para inteiro AAAAMMDD (facilita comparações)
static int parse_date(const char *s) {
    if (!s || strlen(s) < 10) return 0;
    
    int y, m, d;
    if (sscanf(s, "%d-%d-%d", &y, &m, &d) != 3) return 0;
    
    return y * 10000 + m * 100 + d;
}

// Calcula o total estimado do evento (recursos + equipe + fornecedores)
static double calcular_total_estimado(int evento_id) {
    double total = 0.0;

    // Recursos
    EventoItem itens[512];
    int n_itens = pers_carregar_evento_itens(itens, 512);
    for (int i = 0; i < n_itens; i++) {
        if (itens[i].evento_id == evento_id) {
            Recurso r;
            if (recurso_obter(itens[i].recurso_id, &r)) {
                total += r.valor_locacao * itens[i].quantidade;
            }
        }
    }

    // Equipes
    EventoEquipe equipes_aloc[512];
    int n_eq = pers_carregar_evento_equipes(equipes_aloc, 512);
    for (int i = 0; i < n_eq; i++) {
        if (equipes_aloc[i].evento_id == evento_id) {
            Equipe e;
            if (equipe_obter(equipes_aloc[i].equipe_id, &e)) {
                total += e.valor_hora * equipes_aloc[i].horas_trabalhadas;
            }
        }
    }

    // Fornecedores
    EventoFornecedor forn[512];
    int n_for = pers_carregar_evento_fornecedores(forn, 512);
    for (int i = 0; i < n_for; i++) {
        if (forn[i].evento_id == evento_id) {
            total += forn[i].valor_servico;
        }
    }

    return total;
}

// Verifica se dois intervalos de datas se sobrepõem
static int intervalo_conflita(const char *ini1, const char *fim1,
                               const char *ini2, const char *fim2) {
    int a1 = parse_date(ini1);
    int b1 = parse_date(fim1);
    int a2 = parse_date(ini2);
    int b2 = parse_date(fim2);
    
    if (!a1 || !b1 || !a2 || !b2) return 0;
    
    // Dois intervalos se sobrepõem se: início1 <= fim2 E início2 <= fim1
    return (a1 <= b2) && (a2 <= b1);
}


/* ========================================
   FUNÇÕES PRINCIPAIS
   ======================================== */

// Salva ou atualiza um evento (recalcula total automaticamente)
int evento_salvar(Evento e) {
    // Novo evento: inicia como orçamento, sem totais
    if (e.id <= 0) {
        e.status = EVENTO_STATUS_ORCAMENTO;
        e.total_estimado = 0.0;
        e.total_final = 0.0;
        return pers_salvar_evento(e);
    } else {
        // Evento existente: recalcula total estimado com base nas associações atuais
        e.total_estimado = calcular_total_estimado(e.id);
        // total_final só muda em finalização; preserva valor existente
        return pers_salvar_evento(e);
    }
}

int evento_listar(Evento *lista, int max) { return pers_carregar_eventos(lista, max); }
int evento_excluir(int id){ return pers_remover_evento(id); }

// Recalcula total_estimado (e total_final se já finalizado) para um evento existente
int evento_recalcular_totais(int id) {
    if (id <= 0) return 0;
    Evento eventos[512];
    int n = pers_carregar_eventos(eventos, 512);
    for (int i = 0; i < n; i++) {
        if (eventos[i].id == id) {
            double novo_total = calcular_total_estimado(id);
            eventos[i].total_estimado = novo_total;
            if (eventos[i].status == EVENTO_STATUS_FINALIZADO) {
                eventos[i].total_final = novo_total; // final mantém igual
            }
            return pers_salvar_evento(eventos[i]) ? 1 : 0;
        }
    }
    return 0;
}


/* ========================================
   FUNÇÕES DE APROVAÇÃO E FINALIZAÇÃO
   ======================================== */

// Aprova um orçamento (verifica conflitos de agenda antes)
// Retornos:
//  1  = Aprovado com sucesso
//  0  = Evento não encontrado
// -1  = Status inválido (já aprovado ou finalizado)
// -2  = Conflito de recursos/equipes/fornecedores com outro evento
int evento_aprovar(int id) {
    Evento eventos[512];
    int n = pers_carregar_eventos(eventos, 512);
    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (eventos[i].id == id) { idx = i; break; }
    }
    if (idx < 0) return 0;
    if (eventos[idx].status != EVENTO_STATUS_ORCAMENTO) return -1;

    // Carrega alocações deste evento
    EventoItem itens[512];
    int n_itens = pers_carregar_evento_itens(itens, 512);
    EventoEquipe eqs[512];
    int n_eqs = pers_carregar_evento_equipes(eqs, 512);
    EventoFornecedor forn[512];
    int n_forn = pers_carregar_evento_fornecedores(forn, 512);

    // Para cada outro evento aprovado/finalizado, verifica se compartilha
    // recurso/equipe/fornecedor no mesmo intervalo de datas
    for (int j = 0; j < n; j++) {
        if (j == idx) continue;
        if (eventos[j].status != EVENTO_STATUS_APROVADO &&
                eventos[j].status != EVENTO_STATUS_FINALIZADO) continue;

        if (!intervalo_conflita(eventos[idx].data_inicio, eventos[idx].data_fim,
                                                        eventos[j].data_inicio, eventos[j].data_fim))
            continue;

        int conflito = 0;

        // Recursos
        for (int a = 0; a < n_itens && !conflito; a++) {
            if (itens[a].evento_id != id) continue;
            for (int b = 0; b < n_itens; b++) {
                if (itens[b].evento_id != eventos[j].id) continue;
                if (itens[a].recurso_id == itens[b].recurso_id) { conflito = 1; break; }
            }
        }

        // Equipes
        for (int a = 0; a < n_eqs && !conflito; a++) {
            if (eqs[a].evento_id != id) continue;
            for (int b = 0; b < n_eqs; b++) {
                if (eqs[b].evento_id != eventos[j].id) continue;
                if (eqs[a].equipe_id == eqs[b].equipe_id) { conflito = 1; break; }
            }
        }

        // Fornecedores
        for (int a = 0; a < n_forn && !conflito; a++) {
            if (forn[a].evento_id != id) continue;
            for (int b = 0; b < n_forn; b++) {
                if (forn[b].evento_id != eventos[j].id) continue;
                if (forn[a].fornecedor_id == forn[b].fornecedor_id) { conflito = 1; break; }
            }
        }

        if (conflito) {
            return -2; // conflito de agenda/recursos
        }
    }

    // Sem conflitos: calcula total estimado e aprova
    eventos[idx].total_estimado = calcular_total_estimado(eventos[idx].id);
    eventos[idx].status = EVENTO_STATUS_APROVADO;
    return pers_salvar_evento(eventos[idx]) ? 1 : 0;
}

// Finaliza o evento: consolida total_final (pode ser igual ao estimado)
// Retornos: 1 ok, 0 não encontrado, -1 status inválido
int evento_finalizar(int id){
    Evento eventos[512];
    int n = pers_carregar_eventos(eventos, 512);
    for (int i = 0; i < n; i++) {
        if (eventos[i].id == id) {
            if (eventos[i].status != EVENTO_STATUS_APROVADO) return -1;
            eventos[i].total_final = calcular_total_estimado(eventos[i].id);
            eventos[i].status = EVENTO_STATUS_FINALIZADO;
            return pers_salvar_evento(eventos[i]) ? 1 : 0;
        }
    }
    return 0;
}

const char* evento_status_para_str(EventoStatus status) {
    switch (status) {
        case EVENTO_STATUS_ORCAMENTO:  return "Orçamento";
        case EVENTO_STATUS_APROVADO:   return "Aprovado";
        case EVENTO_STATUS_FINALIZADO: return "Finalizado";
        default: return "Desconhecido";
    }
}
