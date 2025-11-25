/*
===============================================================================
   EVENTO VIEW - Interface Gráfica Complexa
   
   Tela de orçamentos e eventos - A MAIS IMPORTANTE DO SISTEMA!
   
   Estrutura:
   - Matriz principal listando todos os eventos
   - Formulário com dados básicos do evento
   - Sistema de ABAS para organizar informações:
     * Aba 1: Dados gerais (nome, datas, cliente, operador, status)
     * Aba 2: Recursos alocados (equipamentos)
     * Aba 3: Equipe alocada (membros e horas)
     * Aba 4: Fornecedores contratados
   
   Funcionalidades:
   - Criar orçamentos
   - Alocar recursos/equipe/fornecedores
   - Calcular valor total automaticamente
   - Aprovar orçamentos (verifica conflitos)
   - Finalizar eventos
   
   Botões principais: Novo, Salvar, Aprovar, Finalizar, Excluir, Atualizar
===============================================================================
*/

#include <iup.h>
#include <iupcontrols.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../controller/evento_controller.h"
#include "../controller/cliente_controller.h"
#include "../controller/operador_controller.h"
#include "../controller/recurso_controller.h"
#include "../controller/equipe_controller.h"
#include "../controller/fornecedor_controller.h"
#include "../model/evento.h"
#include "../model/cliente.h"
#include "../model/operador.h"
#include "../model/evento_item.h"
#include "../model/evento_equipe.h"
#include "../model/evento_fornecedor.h"
#include "ui_common.h"
#include "../model/pers.h"


/* ========================================
   VARIÁVEIS GLOBAIS DA TELA
   ======================================== */

// Componentes principais
static Ihandle *dialog = NULL;
static Ihandle *mat_main = NULL;
static int edit_id = 0;  // ID do evento sendo editado

// Controles do formulário principal
static Ihandle *txt_nome;
static Ihandle *txt_data_ini;
static Ihandle *txt_data_fim;
static Ihandle *drop_cliente;
static Ihandle *drop_operador;

// Labels para exibir totais calculados
static Ihandle *lbl_total_est_text;
static Ihandle *lbl_total_est_val;
static Ihandle *lbl_total_final_text;
static Ihandle *lbl_total_final_val;

// Controles da aba de Recursos
static Ihandle *mat_recursos = NULL;
static Ihandle *lstRecurso = NULL;
static Ihandle *txtRecursoQtd = NULL;

// Controles da aba de Equipes
static Ihandle *mat_equipes = NULL;
static Ihandle *lstEquipe = NULL;
static Ihandle *txtEquipeHoras = NULL;

// Controles da aba de Fornecedores
static Ihandle *mat_fornecedores = NULL;
static Ihandle *lstFornecedor = NULL;
static Ihandle *txtFornecedorValor = NULL;


/* ========================================
   FUNÇÕES AUXILIARES
   ======================================== */

// Forward declarations
static void evento_view_reload_main_matrix(void);
static void carregar_abas(int evento_id);

static int evento_obter(int id, Evento *out) {
    if (id <= 0 || !out) return 0;
    Evento buffer[1024];
    int n = evento_listar(buffer, 1024);
    for (int i = 0; i < n; i++) {
        if (buffer[i].id == id) {
            *out = buffer[i];
            return 1;
        }
    }
    return 0;
}

static void limpar_matriz(Ihandle *mat) {
    if (!mat) return;
    IupSetAttribute(mat, "NUMLIN", "1");
    int col_count = IupGetInt(mat, "NUMCOL");
    for (int i = 1; i <= col_count; i++) {
        IupSetStrAttributeId2(mat, "", 1, i, "");
    }
}

// Salva itens das matrizes. Convenção: col1 = ID, col2 = descrição, col3 = qtd/horas/valor
static void salvar_itens_matriz(Ihandle* matrix, int evento_id, int tipo) {
    if (!matrix || evento_id <= 0) return;

    int linhas = IupGetInt(matrix, "NUMLIN");
    if (linhas <= 0) return;
    
    for (int i = 1; i <= linhas; i++) {
        const char *c_id = IupGetAttributeId2(matrix, "", i, 1);
        const char *c_val = IupGetAttributeId2(matrix, "", i, 3);

        if (!c_id || c_id[0] == '\0') continue;
        if (!c_val || c_val[0] == '\0') continue;

        int item_id = atoi(c_id);
        if (item_id <= 0) continue;

        switch (tipo) {
            case 0: { // Recursos
                EventoItem it = {0};
                it.evento_id = evento_id;
                it.recurso_id = item_id;
                it.quantidade = atoi(c_val);
                if (it.quantidade > 0) pers_salvar_evento_item(it);
                break;
            }
            case 1: { // Equipes
                EventoEquipe ee = {0};
                ee.evento_id = evento_id;
                ee.equipe_id = item_id;
                ee.horas_trabalhadas = atof(c_val);
                if (ee.horas_trabalhadas > 0) pers_salvar_evento_equipe(ee);
                break;
            }
            case 2: { // Fornecedores
                EventoFornecedor ef = {0};
                ef.evento_id = evento_id;
                ef.fornecedor_id = item_id;
                ef.valor_servico = atof(c_val);
                if (ef.valor_servico > 0) pers_salvar_evento_fornecedor(ef);
                break;
            }
        }
    }
}

static void repopular_dropdowns() {
    char buffer[256];

    // Clientes
    Cliente clientes[256];
    int n_cli = cliente_listar(clientes, 256);
    IupSetAttribute(drop_cliente, "REMOVEITEM", "ALL");
    IupSetAttribute(drop_cliente, "1", "(Selecione)");
    for (int i = 0; i < n_cli; i++) {
        snprintf(buffer, sizeof(buffer), "%d - %s", clientes[i].id, clientes[i].nome);
        IupSetStrAttributeId(drop_cliente, "", i + 2, buffer);
    }
    IupSetAttribute(drop_cliente, "VALUE", "1");

    // Operadores
    Operador operadores[256];
    int n_op = operador_listar(operadores, 256);
    IupSetAttribute(drop_operador, "REMOVEITEM", "ALL");
    IupSetAttribute(drop_operador, "1", "(Selecione)");
    for (int i = 0; i < n_op; i++) {
        snprintf(buffer, sizeof(buffer), "%d - %s", operadores[i].id, operadores[i].nome);
        IupSetStrAttributeId(drop_operador, "", i + 2, buffer);
    }
    IupSetAttribute(drop_operador, "VALUE", "1");

    // Recursos
    Recurso recursos[256];
    int n_rec = recurso_listar(recursos, 256);
    IupSetAttribute(lstRecurso, "REMOVEITEM", "ALL");
    for (int i = 0; i < n_rec; i++) {
        snprintf(buffer, sizeof(buffer), "%d - %s", recursos[i].id, recursos[i].descricao);
        IupSetStrAttributeId(lstRecurso, "", i + 1, buffer);
    }
    IupSetAttribute(lstRecurso, "VALUE", "1");

    // Equipes
    Equipe equipes[256];
    int n_eq = equipe_listar(equipes, 256);
    IupSetAttribute(lstEquipe, "REMOVEITEM", "ALL");
    for (int i = 0; i < n_eq; i++) {
        snprintf(buffer, sizeof(buffer), "%d - %s", equipes[i].id, equipes[i].nome);
        IupSetStrAttributeId(lstEquipe, "", i + 1, buffer);
    }
    IupSetAttribute(lstEquipe, "VALUE", "1");

    // Fornecedores
    Fornecedor fornecedores[256];
    int n_for = fornecedor_listar(fornecedores, 256);
    IupSetAttribute(lstFornecedor, "REMOVEITEM", "ALL");
    for (int i = 0; i < n_for; i++) {
        snprintf(buffer, sizeof(buffer), "%d - %s", fornecedores[i].id, fornecedores[i].nome_fantasia);
        IupSetStrAttributeId(lstFornecedor, "", i + 1, buffer);
    }
    IupSetAttribute(lstFornecedor, "VALUE", "1");
}

// =============================================================================
// Callbacks
// =============================================================================

static int evento_novo_cb(Ihandle *self) {
    edit_id = 0;
    IupSetAttribute(txt_nome, "VALUE", "");
    IupSetAttribute(txt_data_ini, "VALUE", "");
    IupSetAttribute(txt_data_fim, "VALUE", "");
    IupSetAttribute(drop_cliente, "VALUE", "0");
    IupSetAttribute(drop_operador, "VALUE", "0");
    IupSetAttribute(lbl_total_est_val, "TITLE", "R$ 0.00");
    IupSetAttribute(lbl_total_final_val, "TITLE", "R$ 0.00");

    limpar_matriz(mat_recursos);
    limpar_matriz(mat_equipes);
    limpar_matriz(mat_fornecedores);

    const char *active = "YES";
    IupSetAttribute(txt_nome, "ACTIVE", active);
    IupSetAttribute(txt_data_ini, "ACTIVE", active);
    IupSetAttribute(txt_data_fim, "ACTIVE", active);
    IupSetAttribute(drop_cliente, "ACTIVE", active);
    IupSetAttribute(drop_operador, "ACTIVE", active);
    
    IupSetAttribute(IupGetParent(lstRecurso), "ACTIVE", active);
    IupSetAttribute(IupGetParent(lstEquipe), "ACTIVE", active);
    IupSetAttribute(IupGetParent(lstFornecedor), "ACTIVE", active);

    IupSetFocus(txt_nome);
    return IUP_DEFAULT;
}

static int evento_salvar_cb(Ihandle *self) {
    char *nome = IupGetAttribute(txt_nome, "VALUE");
    char *data_ini = IupGetAttribute(txt_data_ini, "VALUE");
    char *data_fim = IupGetAttribute(txt_data_fim, "VALUE");
    int cliente_idx = IupGetInt(drop_cliente, "VALUE");
    int operador_idx = IupGetInt(drop_operador, "VALUE");

    if (!nome || !data_ini || !data_fim || strlen(nome) == 0 || strlen(data_ini) < 10 || strlen(data_fim) < 10 || cliente_idx < 1 || operador_idx < 1) {
        IupMessage("Erro", "Preencha Nome, Datas, Cliente e Operador.");
        return IUP_DEFAULT;
    }

    Evento e = {0};
    e.id = edit_id;
    strncpy(e.nome, nome, sizeof(e.nome) - 1);
    e.nome[sizeof(e.nome) - 1] = '\0';
    // Normaliza datas: troca '/' por '-'
    for (int i = 0; data_ini[i]; ++i) if (data_ini[i] == '/') data_ini[i] = '-';
    for (int i = 0; data_fim[i]; ++i) if (data_fim[i] == '/') data_fim[i] = '-';
    strncpy(e.data_inicio, data_ini, sizeof(e.data_inicio) - 1);
    e.data_inicio[sizeof(e.data_inicio) - 1] = '\0';
    strncpy(e.data_fim, data_fim, sizeof(e.data_fim) - 1);
    e.data_fim[sizeof(e.data_fim) - 1] = '\0';
    e.local[0] = '\0'; /* inicializa local vazio */
    e.tipo = EVENTO_TIPO_ORCAMENTO; /* sempre orçamento */
    e.status = EVENTO_STATUS_ORCAMENTO; /* sempre status orçamento ao criar/editar */

    /* recupera ids a partir do texto "id - nome" nas listas */
    if (cliente_idx <= 0 || operador_idx <= 0 || cliente_idx > 256 || operador_idx > 256) {
        IupMessage("Erro", "Índice de Cliente ou Operador inválido.");
        return IUP_DEFAULT;
    }
    
    char cli_idx_str[16], op_idx_str[16];
    sprintf(cli_idx_str, "%d", cliente_idx);
    sprintf(op_idx_str, "%d", operador_idx);
    const char* cli_text = IupGetAttribute(drop_cliente, cli_idx_str);
    const char* op_text  = IupGetAttribute(drop_operador, op_idx_str);
    
    if (!cli_text || !op_text) {
        IupMessage("Erro", "Cliente ou Operador inválido.");
        return IUP_DEFAULT;
    }
    
    int cli_id = atoi(cli_text);
    int op_id = atoi(op_text);
    
    if (cli_id <= 0 || op_id <= 0) {
        IupMessage("Erro", "IDs de Cliente ou Operador inválidos.");
        return IUP_DEFAULT;
    }
    
    e.cliente_id  = cli_id;
    e.operador_id = op_id;
    e.produtora_id = 1;

    int saved_id = evento_salvar(e);
    if (saved_id == 0) {
        IupMessage("Erro", "Falha ao salvar o evento principal.");
        return IUP_DEFAULT;
    }
    
    // Atualiza edit_id IMEDIATAMENTE para garantir correlação
    edit_id = saved_id;

    // Remove itens antigos e salva novos
    pers_remover_evento_itens_por_evento(edit_id);
    pers_remover_evento_equipes_por_evento(edit_id);
    pers_remover_evento_fornecedores_por_evento(edit_id);

    salvar_itens_matriz(mat_recursos, edit_id, 0);
    salvar_itens_matriz(mat_equipes, edit_id, 1);
    salvar_itens_matriz(mat_fornecedores, edit_id, 2);

    // Recalcula totais após salvar associações
    evento_recalcular_totais(edit_id);
    if (evento_obter(edit_id, &e)) {
        char buf[64];
        sprintf(buf, "R$ %.2f", e.total_estimado);
        IupSetAttribute(lbl_total_est_val, "TITLE", buf);
        sprintf(buf, "R$ %.2f", e.total_final);
        IupSetAttribute(lbl_total_final_val, "TITLE", buf);
    }

    evento_view_reload_main_matrix();
    IupMessage("Sucesso", "Evento salvo com sucesso!");
    return IUP_DEFAULT;
}

static int evento_excluir_cb(Ihandle *self) {
    if (edit_id <= 0) {
        IupMessage("Aviso", "Nenhum evento selecionado para excluir.");
        return IUP_DEFAULT;
    }
    if (IupAlarm("Confirmação", "Deseja realmente excluir este evento e todas as suas associações?", "Sim", "Não", NULL) != 1) {
        return IUP_DEFAULT;
    }

    pers_remover_evento_itens_por_evento(edit_id);
    pers_remover_evento_equipes_por_evento(edit_id);
    pers_remover_evento_fornecedores_por_evento(edit_id);
    
    if (evento_excluir(edit_id)) {
        IupMessage("Sucesso", "Evento excluído.");
        evento_novo_cb(NULL); // Limpa o formulário
        evento_view_reload_main_matrix();
    } else {
        IupMessage("Erro", "Falha ao excluir o evento.");
    }
    return IUP_DEFAULT;
}

static void aplicar_regra_status(Evento *e);

static int evento_aprovar_cb(Ihandle *self) {
    if (edit_id <= 0) {
        IupMessage("Aviso", "Selecione um evento para aprovar.");
        return IUP_DEFAULT;
    }

    int ret = evento_aprovar(edit_id);
    if (ret == 1) {
        Evento e;
        if (evento_obter(edit_id, &e)) {
            char buf[64];
            sprintf(buf, "R$ %.2f", e.total_estimado);
            IupSetAttribute(lbl_total_est_val, "TITLE", buf);
            aplicar_regra_status(&e);
        }
        IupMessage("Sucesso", "Evento aprovado com sucesso.");
    } else if (ret == -2) {
        IupMessage("Conflito", "Há conflito de recursos/equipes/fornecedores em eventos já aprovados no mesmo período.");
    } else if (ret == -1) {
        IupMessage("Aviso", "Este evento não está em status de orçamento.");
    } else {
        IupMessage("Erro", "Falha ao aprovar o evento.");
    }

    evento_view_reload_main_matrix();
    return IUP_DEFAULT;
}

static int evento_finalizar_cb(Ihandle *self) {
    if (edit_id <= 0) {
        IupMessage("Aviso", "Selecione um evento para finalizar.");
        return IUP_DEFAULT;
    }

    int ret = evento_finalizar(edit_id);
    if (ret == 1) {
        Evento e;
        if (evento_obter(edit_id, &e)) {
            char buf[64];
            sprintf(buf, "R$ %.2f", e.total_estimado);
            IupSetAttribute(lbl_total_est_val, "TITLE", buf);
            sprintf(buf, "R$ %.2f", e.total_final);
            IupSetAttribute(lbl_total_final_val, "TITLE", buf);
            aplicar_regra_status(&e);
        }
        IupMessage("Sucesso", "Evento finalizado com sucesso.");
    } else if (ret == -1) {
        IupMessage("Aviso", "Somente eventos aprovados podem ser finalizados.");
    } else {
        IupMessage("Erro", "Falha ao finalizar o evento.");
    }

    evento_view_reload_main_matrix();
    return IUP_DEFAULT;
}

static int matrix_click_cb(Ihandle *ih, int lin, int col) {
    if (lin <= 0) return IUP_DEFAULT;
    edit_id = IupGetIntId2(ih, "", lin, 1);
    
    Evento e;
    if (evento_obter(edit_id, &e)) {
        IupSetAttribute(txt_nome, "VALUE", e.nome);
        IupSetAttribute(txt_data_ini, "VALUE", e.data_inicio);
        IupSetAttribute(txt_data_fim, "VALUE", e.data_fim);
        IupSetfAttribute(lbl_total_est_val, "TITLE", "R$ %.2f", e.total_estimado);
        IupSetfAttribute(lbl_total_final_val, "TITLE", "R$ %.2f", e.total_final);

        /* reposiciona combos de cliente e operador com base no id */
        int total_cli = IupGetInt(drop_cliente, "COUNT");
        for (int i = 2; i <= total_cli; i++) {
            char idx_str[16];
            sprintf(idx_str, "%d", i);
            const char* txt = IupGetAttribute(drop_cliente, idx_str);
            if (txt && atoi(txt) == e.cliente_id) {
                IupSetInt(drop_cliente, "VALUE", i);
                break;
            }
        }

        int total_op = IupGetInt(drop_operador, "COUNT");
        for (int i = 2; i <= total_op; i++) {
            char idx_str[16];
            sprintf(idx_str, "%d", i);
            const char* txt = IupGetAttribute(drop_operador, idx_str);
            if (txt && atoi(txt) == e.operador_id) {
                IupSetInt(drop_operador, "VALUE", i);
                break;
            }
        }

        carregar_abas(edit_id);
        aplicar_regra_status(&e);
    }
    return IUP_DEFAULT;
}

static void aplicar_regra_status(Evento *e) {
    if (!e) return;

    const char *atv_orc = "YES";
    const char *atv_itens = "YES";

    if (e->status == EVENTO_STATUS_APROVADO) {
        atv_orc = "NO";   // orçamento travado
        atv_itens = "NO"; // itens também
    } else if (e->status == EVENTO_STATUS_FINALIZADO) {
        atv_orc = "NO";
        atv_itens = "NO";
    }

    IupSetAttribute(txt_nome, "ACTIVE", atv_orc);
    IupSetAttribute(txt_data_ini, "ACTIVE", atv_orc);
    IupSetAttribute(txt_data_fim, "ACTIVE", atv_orc);
    IupSetAttribute(drop_cliente, "ACTIVE", atv_orc);
    IupSetAttribute(drop_operador, "ACTIVE", atv_orc);

    IupSetAttribute(IupGetParent(lstRecurso), "ACTIVE", atv_itens);
    IupSetAttribute(IupGetParent(lstEquipe), "ACTIVE", atv_itens);
    IupSetAttribute(IupGetParent(lstFornecedor), "ACTIVE", atv_itens);
}

// --- Callbacks das abas ---

static int btn_recurso_add_cb(Ihandle *self) {
    int sel_idx = IupGetInt(lstRecurso, "VALUE");
    if (sel_idx <= 0) return IUP_DEFAULT;
    
    char idx_str[16];
    sprintf(idx_str, "%d", sel_idx);
    const char* item_text = IupGetAttribute(lstRecurso, idx_str);
    if (!item_text) return IUP_DEFAULT;
    
    int item_id = atoi(item_text); /* formato: id - descricao */
    if (item_id <= 0) return IUP_DEFAULT;
    const char* qtd_str = IupGetAttribute(txtRecursoQtd, "VALUE");
    int qtd = qtd_str ? atoi(qtd_str) : 0;
    if (qtd <= 0) {
        IupMessage("Erro", "A quantidade deve ser maior que zero.");
        return IUP_DEFAULT;
    }

    int linhas = IupGetInt(mat_recursos, "NUMLIN");
    const char* primeira_cel = IupGetAttributeId2(mat_recursos, "", 1, 1);
    int nova_linha = (linhas == 1 && (!primeira_cel || primeira_cel[0] == '\0')) ? 1 : linhas + 1;

    IupSetInt(mat_recursos, "NUMLIN", nova_linha);
    IupSetIntId2(mat_recursos, "", nova_linha, 1, item_id);
    IupSetStrAttributeId2(mat_recursos, "", nova_linha, 2, item_text);
    IupSetIntId2(mat_recursos, "", nova_linha, 3, qtd);
    IupSetAttribute(mat_recursos, "REDRAW", "ALL");
    return IUP_DEFAULT;
}

static int btn_recurso_del_cb(Ihandle *self) {
    int focus_lin = IupGetInt(mat_recursos, "FOCUS_CELL_LIN");
    if (focus_lin > 0) {
        IupSetAttributeId2(mat_recursos, "DELROW", focus_lin, 0, NULL);
        if (IupGetInt(mat_recursos, "NUMLIN") == 0) {
            limpar_matriz(mat_recursos);
        }
    }
    return IUP_DEFAULT;
}

static int btn_equipe_add_cb(Ihandle *self) {
    int sel_idx = IupGetInt(lstEquipe, "VALUE");
    if (sel_idx <= 0) return IUP_DEFAULT;
    
    char idx_str[16];
    sprintf(idx_str, "%d", sel_idx);
    const char* item_text = IupGetAttribute(lstEquipe, idx_str);
    if (!item_text) return IUP_DEFAULT;
    
    int item_id = atoi(item_text);
    if (item_id <= 0) return IUP_DEFAULT;
    const char* horas_str = IupGetAttribute(txtEquipeHoras, "VALUE");
    double horas = horas_str ? atof(horas_str) : 0.0;
    if (horas <= 0) {
        IupMessage("Erro", "As horas devem ser maiores que zero.");
        return IUP_DEFAULT;
    }

    int linhas = IupGetInt(mat_equipes, "NUMLIN");
    const char* primeira_cel = IupGetAttributeId2(mat_equipes, "", 1, 1);
    int nova_linha = (linhas == 1 && (!primeira_cel || primeira_cel[0] == '\0')) ? 1 : linhas + 1;

    IupSetInt(mat_equipes, "NUMLIN", nova_linha);
    IupSetIntId2(mat_equipes, "", nova_linha, 1, item_id);
    IupSetStrAttributeId2(mat_equipes, "", nova_linha, 2, item_text);
    IupSetfAttributeId2(mat_equipes, "", nova_linha, 3, "%.2f", horas);
    IupSetAttribute(mat_equipes, "REDRAW", "ALL");
    return IUP_DEFAULT;
}

static int btn_equipe_del_cb(Ihandle *self) {
    int focus_lin = IupGetInt(mat_equipes, "FOCUS_CELL_LIN");
    if (focus_lin > 0) {
        IupSetAttributeId2(mat_equipes, "DELROW", focus_lin, 0, NULL);
        if (IupGetInt(mat_equipes, "NUMLIN") == 0) {
            limpar_matriz(mat_equipes);
        }
    }
    return IUP_DEFAULT;
}

static int btn_fornecedor_add_cb(Ihandle *self) {
    int sel_idx = IupGetInt(lstFornecedor, "VALUE");
    if (sel_idx <= 0) return IUP_DEFAULT;
    
    char idx_str[16];
    sprintf(idx_str, "%d", sel_idx);
    const char* item_text = IupGetAttribute(lstFornecedor, idx_str);
    if (!item_text) return IUP_DEFAULT;
    
    int item_id = atoi(item_text);
    if (item_id <= 0) return IUP_DEFAULT;
    const char* valor_str = IupGetAttribute(txtFornecedorValor, "VALUE");
    double valor = valor_str ? atof(valor_str) : 0.0;
    if (valor <= 0) {
        IupMessage("Erro", "O valor deve ser maior que zero.");
        return IUP_DEFAULT;
    }

    int linhas = IupGetInt(mat_fornecedores, "NUMLIN");
    const char* primeira_cel = IupGetAttributeId2(mat_fornecedores, "", 1, 1);
    int nova_linha = (linhas == 1 && (!primeira_cel || primeira_cel[0] == '\0')) ? 1 : linhas + 1;

    IupSetInt(mat_fornecedores, "NUMLIN", nova_linha);
    IupSetIntId2(mat_fornecedores, "", nova_linha, 1, item_id);
    IupSetStrAttributeId2(mat_fornecedores, "", nova_linha, 2, item_text);
    IupSetfAttributeId2(mat_fornecedores, "", nova_linha, 3, "%.2f", valor);
    IupSetAttribute(mat_fornecedores, "REDRAW", "ALL");
    return IUP_DEFAULT;
}

static int btn_fornecedor_del_cb(Ihandle *self) {
    int focus_lin = IupGetInt(mat_fornecedores, "FOCUS_CELL_LIN");
    if (focus_lin > 0) {
        IupSetAttributeId2(mat_fornecedores, "DELROW", focus_lin, 0, NULL);
        if (IupGetInt(mat_fornecedores, "NUMLIN") == 0) {
            limpar_matriz(mat_fornecedores);
        }
    }
    return IUP_DEFAULT;
}

// =============================================================================
// Funções de Construção da UI
// =============================================================================

static void carregar_abas(int evento_id) {
    limpar_matriz(mat_recursos);
    limpar_matriz(mat_equipes);
    limpar_matriz(mat_fornecedores);

    if (evento_id <= 0 || !mat_recursos || !mat_equipes || !mat_fornecedores) return;

    // Carregar Recursos
    EventoItem itens[256];
    int n_itens = pers_carregar_evento_itens(itens, 256);
    int linha_rec = 0;
    for (int i = 0; i < n_itens; i++) {
        if (itens[i].evento_id == evento_id) {
            Recurso r;
            if (recurso_obter(itens[i].recurso_id, &r)) {
                linha_rec++;
                IupSetInt(mat_recursos, "NUMLIN", linha_rec);
                IupSetIntId2(mat_recursos, "", linha_rec, 1, r.id);
                IupSetStrAttributeId2(mat_recursos, "", linha_rec, 2, r.descricao);
                IupSetIntId2(mat_recursos, "", linha_rec, 3, itens[i].quantidade);
            }
        }
    }

    // Carregar Equipes
    EventoEquipe equipes[128];
    int n_eq = pers_carregar_evento_equipes(equipes, 128);
    int linha_eq = 0;
    for (int i = 0; i < n_eq; i++) {
        if (equipes[i].evento_id == evento_id) {
            Equipe e;
            if (equipe_obter(equipes[i].equipe_id, &e)) {
                linha_eq++;
                IupSetInt(mat_equipes, "NUMLIN", linha_eq);
                IupSetIntId2(mat_equipes, "", linha_eq, 1, e.id);
                IupSetStrAttributeId2(mat_equipes, "", linha_eq, 2, e.nome);
                IupSetfAttributeId2(mat_equipes, "", linha_eq, 3, "%.2f", equipes[i].horas_trabalhadas);
            }
        }
    }

    // Carregar Fornecedores
    EventoFornecedor fornecedores[128];
    int n_for = pers_carregar_evento_fornecedores(fornecedores, 128);
    int linha_for = 0;
    for (int i = 0; i < n_for; i++) {
        if (fornecedores[i].evento_id == evento_id) {
            Fornecedor f;
            if (fornecedor_obter(fornecedores[i].fornecedor_id, &f)) {
                linha_for++;
                IupSetInt(mat_fornecedores, "NUMLIN", linha_for);
                IupSetIntId2(mat_fornecedores, "", linha_for, 1, f.id);
                IupSetStrAttributeId2(mat_fornecedores, "", linha_for, 2, f.nome_fantasia);
                IupSetfAttributeId2(mat_fornecedores, "", linha_for, 3, "%.2f", fornecedores[i].valor_servico);
            }
        }
    }
}

static void evento_view_reload_main_matrix(void) {
    if (!mat_main) return;
    Evento eventos[1024];
    int n = evento_listar(eventos, 1024);
    printf("[DEBUG] evento_view_reload_main_matrix: n=%d\n", n);
    IupSetInt(mat_main, "NUMLIN", n > 0 ? n : 1);

    if (n <= 0) {
        limpar_matriz(mat_main);
        IupSetAttribute(mat_main, "REDRAW", "ALL");
        return;
    }

    Cliente clientes[256];
    int n_cli = cliente_listar(clientes, 256);
    printf("[DEBUG] clientes carregados: %d\n", n_cli);

    for (int i = 0; i < n; i++) {
        int lin = i + 1;
        IupSetIntId2(mat_main, "", lin, 1, eventos[i].id);
        char cli_nome[100] = "(não encontrado)";
        for (int j = 0; j < n_cli; j++) {
            if (clientes[j].id == eventos[i].cliente_id) {
                strncpy(cli_nome, clientes[j].nome, sizeof(cli_nome) - 1);
                cli_nome[sizeof(cli_nome)-1] = '\0';
                break;
            }
        }
        IupSetStrAttributeId2(mat_main, "", lin, 2, eventos[i].nome);
        IupSetStrAttributeId2(mat_main, "", lin, 3, cli_nome);
        IupSetStrAttributeId2(mat_main, "", lin, 4, eventos[i].data_inicio);
        IupSetStrAttributeId2(mat_main, "", lin, 5, eventos[i].data_fim);
        IupSetfAttributeId2(mat_main, "", lin, 6, "R$ %.2f", eventos[i].total_estimado);
        IupSetStrAttributeId2(mat_main, "", lin, 7, evento_status_para_str(eventos[i].status));
        printf("[DEBUG] Linha %d - ID=%d Nome=%s Cliente=%s Total=%.2f Status=%d\n", lin, eventos[i].id, eventos[i].nome, cli_nome, eventos[i].total_estimado, eventos[i].status);
    }
    IupSetAttribute(mat_main, "REDRAW", "ALL");
}

static Ihandle* evento_view_create(void) {
    // Formulário Principal
    txt_nome = IupText(NULL);
    txt_data_ini = IupDatePick();
    txt_data_fim = IupDatePick();
    drop_cliente = IupList(NULL);
    drop_operador = IupList(NULL);
    lbl_total_est_text = IupLabel("Total Estimado:");
    lbl_total_est_val  = IupLabel("R$ 0.00");
    lbl_total_final_text = IupLabel("Total Final:");
    lbl_total_final_val  = IupLabel("R$ 0.00");
    // Fontes ajustadas (padronização <= 12, destaque moderado nos valores)
    IupSetAttribute(lbl_total_est_text, "FONT", "Arial, Bold 11");
    IupSetAttribute(lbl_total_final_text, "FONT", "Arial, Bold 11");
    IupSetAttribute(lbl_total_est_val, "FONT", "Arial, Bold 12");
    IupSetAttribute(lbl_total_final_val, "FONT", "Arial, Bold 12");
    IupSetAttribute(lbl_total_est_val, "RASTERSIZE", "160x24");
    IupSetAttribute(lbl_total_final_val, "RASTERSIZE", "160x24");
    IupSetAttribute(lbl_total_est_val, "FGCOLOR", "0 80 0");
    IupSetAttribute(lbl_total_final_val, "FGCOLOR", "0 0 120");

    IupSetAttribute(txt_data_ini, "MASK", "2000-99-99");
    IupSetAttribute(txt_data_fim, "MASK", "2000-99-99");
    IupSetAttributes(drop_cliente, "DROPDOWN=YES, VISIBLE_ITEMS=5");
    IupSetAttributes(drop_operador, "DROPDOWN=YES, VISIBLE_ITEMS=5");

    // Aba de Recursos
    lstRecurso = IupList(NULL);
    txtRecursoQtd = IupText(NULL);
    IupSetAttributes(lstRecurso, "DROPDOWN=YES, VISIBLE_ITEMS=5");
    IupSetAttribute(txtRecursoQtd, "MASK", IUP_MASK_UINT);
    mat_recursos = IupMatrix(NULL);
    IupSetAttributes(mat_recursos, "NUMCOL=3, NUMLIN=1, RESIZEMATRIX=YES, EXPAND=YES");
    IupSetAttributes(mat_recursos, "0:1=ID, 0:2=Recurso, 0:3=Qtd");

    // Aba de Equipes
    lstEquipe = IupList(NULL);
    txtEquipeHoras = IupText(NULL);
    IupSetAttributes(lstEquipe, "DROPDOWN=YES, VISIBLE_ITEMS=5");
    IupSetAttribute(txtEquipeHoras, "MASK", IUP_MASK_FLOAT);
    mat_equipes = IupMatrix(NULL);
    IupSetAttributes(mat_equipes, "NUMCOL=3, NUMLIN=1, RESIZEMATRIX=YES, EXPAND=YES");
    IupSetAttributes(mat_equipes, "0:1=ID, 0:2=Equipe, 0:3=Horas");

    // Aba de Fornecedores
    lstFornecedor = IupList(NULL);
    txtFornecedorValor = IupText(NULL);
    IupSetAttributes(lstFornecedor, "DROPDOWN=YES, VISIBLE_ITEMS=5");
    IupSetAttribute(txtFornecedorValor, "MASK", IUP_MASK_FLOAT);
    mat_fornecedores = IupMatrix(NULL);
    IupSetAttributes(mat_fornecedores, "NUMCOL=3, NUMLIN=1, RESIZEMATRIX=YES, EXPAND=YES");
    IupSetAttributes(mat_fornecedores, "0:1=ID, 0:2=Fornecedor, 0:3=Valor (R$)");

    // Montagem das Abas
    Ihandle *btn_rec_add  = IupButton("Add", NULL);
    Ihandle *btn_rec_del  = IupButton("Del", NULL);
    Ihandle *btn_eqp_add  = IupButton("Add", NULL);
    Ihandle *btn_eqp_del  = IupButton("Del", NULL);
    Ihandle *btn_forn_add = IupButton("Add", NULL);
    Ihandle *btn_forn_del = IupButton("Del", NULL);

    Ihandle *toolbar_rec = IupHbox(ui_pair("Recurso:", lstRecurso), ui_pair("Qtd:", txtRecursoQtd), btn_rec_add, btn_rec_del, NULL);
    Ihandle *toolbar_eqp = IupHbox(ui_pair("Equipe:", lstEquipe), ui_pair("Horas:", txtEquipeHoras), btn_eqp_add, btn_eqp_del, NULL);
    Ihandle *toolbar_forn = IupHbox(ui_pair("Fornecedor:", lstFornecedor), ui_pair("Valor:", txtFornecedorValor), btn_forn_add, btn_forn_del, NULL);
    
    Ihandle *tab_recursos = IupVbox(toolbar_rec, mat_recursos, NULL);
    Ihandle *tab_equipes = IupVbox(toolbar_eqp, mat_equipes, NULL);
    Ihandle *tab_fornecedores = IupVbox(toolbar_forn, mat_fornecedores, NULL);

    Ihandle *tabs = IupTabs(tab_recursos, tab_equipes, tab_fornecedores, NULL);
    IupSetAttribute(tab_recursos, "TABTITLE", "Recursos");
    IupSetAttribute(tab_equipes, "TABTITLE", "Equipes");
    IupSetAttribute(tab_fornecedores, "TABTITLE", "Fornecedores");

    // Matriz Principal
    mat_main = IupMatrix(NULL);
    IupSetAttributes(mat_main, "NUMCOL=7, NUMLIN=1, EXPAND=YES, RESIZEMATRIX=YES");
    IupSetAttributes(mat_main, "0:1=ID, 0:2=Nome, 0:3=Cliente, 0:4=Início, 0:5=Fim, 0:6=Total Est., 0:7=Status");
    IupSetAttributes(mat_main, "WIDTH1=40, WIDTH3=150, RASTERWIDTH2=200");

    // Botões
    Ihandle *btn_novo = IupButton("Novo", NULL);
    Ihandle *btn_salvar = IupButton("Salvar", NULL);
    Ihandle *btn_aprovar = IupButton("Aprovar", NULL);
    Ihandle *btn_finalizar = IupButton("Finalizar", NULL);
    Ihandle *btn_excluir = IupButton("Excluir", NULL);

    // Layout
    Ihandle* form_grid = IupGridBox(
        IupLabel("Nome:"), txt_nome, IupLabel("Início:"), txt_data_ini,
        IupLabel("Fim:"), txt_data_fim, IupLabel("Cliente:"), drop_cliente,
        IupLabel("Operador:"), drop_operador,
        lbl_total_est_text, lbl_total_est_val, lbl_total_final_text, lbl_total_final_val,
        NULL);
    IupSetAttribute(form_grid, "NUMCOL", "4");
    IupSetAttribute(form_grid, "GAPLIN", "5");
    IupSetAttribute(form_grid, "GAPCOL", "5");
    
    Ihandle *botoes_form = IupHbox(IupFill(), btn_novo, btn_salvar, btn_aprovar, btn_finalizar, btn_excluir, NULL);
    Ihandle *vbox_left = IupVbox(form_grid, IupSetAttributes(IupFrame(tabs), "TITLE=Itens do Evento"), botoes_form, NULL);
    Ihandle *vbox_right = IupVbox(IupSetAttributes(IupFrame(mat_main), "TITLE=Lista de Eventos"), NULL);
    Ihandle *hbox_main = IupHbox(vbox_left, vbox_right, NULL);
    IupSetAttribute(hbox_main, "MARGIN", "5x5");
    IupSetAttribute(hbox_main, "GAP", "5");

    // Callbacks
    IupSetCallback(btn_rec_add,  "ACTION", (Icallback)btn_recurso_add_cb);
    IupSetCallback(btn_rec_del,  "ACTION", (Icallback)btn_recurso_del_cb);
    IupSetCallback(btn_eqp_add,  "ACTION", (Icallback)btn_equipe_add_cb);
    IupSetCallback(btn_eqp_del,  "ACTION", (Icallback)btn_equipe_del_cb);
    IupSetCallback(btn_forn_add, "ACTION", (Icallback)btn_fornecedor_add_cb);
    IupSetCallback(btn_forn_del, "ACTION", (Icallback)btn_fornecedor_del_cb);
    IupSetCallback(mat_main, "CLICK_CB", (Icallback)matrix_click_cb);
    IupSetCallback(btn_novo, "ACTION", (Icallback)evento_novo_cb);
    IupSetCallback(btn_salvar, "ACTION", (Icallback)evento_salvar_cb);
    IupSetCallback(btn_aprovar, "ACTION", (Icallback)evento_aprovar_cb);
    IupSetCallback(btn_finalizar, "ACTION", (Icallback)evento_finalizar_cb);
    IupSetCallback(btn_excluir, "ACTION", (Icallback)evento_excluir_cb);

    return hbox_main;
}

void evento_view_mostrar(void) {
    if (!dialog) {
        dialog = IupDialog(evento_view_create());
        IupSetAttribute(dialog, "TITLE", "Gestão de Eventos");
        // tamanho mais contido; deixa o usuário redimensionar depois
        IupSetAttribute(dialog, "SIZE", "900x550");
        IupSetAttribute(dialog, "RESIZE", "YES");
        IupSetAttribute(dialog, "MAXBOX", "YES");
    }
    repopular_dropdowns();
    evento_view_reload_main_matrix();
    IupShowXY(dialog, IUP_CENTER, IUP_CENTER);
}
