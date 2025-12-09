

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iup.h>
#include <iupcontrols.h>
#include "../controller/feedback_controller.h"
#include "../model/pers.h"
#include "../model/cliente.h"
#include "../model/evento.h"
#include "../model/recurso.h"
#include "../model/transacoes.h"



static Ihandle *dlg_feedback = NULL;
static Ihandle *list_relatorios = NULL;
static Ihandle *txt_resultado = NULL;
static Ihandle *box_filtros = NULL;

// Variáveis para filtros
static Ihandle *txt_cod_min = NULL;
static Ihandle *txt_cod_max = NULL;
static Ihandle *chk_ordem_alfa = NULL;

static Ihandle *list_clientes = NULL;
static Ihandle *txt_data_min = NULL;
static Ihandle *txt_data_max = NULL;
static Ihandle *list_status_evento = NULL;

static Ihandle *list_recursos = NULL;
static Ihandle *txt_data_min_rec = NULL;
static Ihandle *txt_data_max_rec = NULL;

static Ihandle *txt_cat_equip = NULL;

static Ihandle *list_clientes_cr = NULL;
static Ihandle *txt_data_min_cr = NULL;
static Ihandle *txt_data_max_cr = NULL;

static Ihandle *list_fornecedores = NULL;
static Ihandle *txt_data_min_cp = NULL;
static Ihandle *txt_data_max_cp = NULL;

static Ihandle *txt_data_min_caixa = NULL;
static Ihandle *txt_data_max_caixa = NULL;


// Callback para limpar área de resultado
static int limpar_resultado(void) {
    IupSetAttribute(txt_resultado, "VALUE", "");
    return IUP_DEFAULT;
}

// Callback para adicionar linha de texto ao resultado
static void adicionar_linha_resultado(const char *linha) {
    if (!txt_resultado || !linha) return;
    
    const char *texto_atual = IupGetAttribute(txt_resultado, "VALUE");
    char buffer[8192];
    
    if (texto_atual && strlen(texto_atual) > 0) {
        snprintf(buffer, sizeof(buffer) - 1, "%s\n%s", texto_atual, linha);
    } else {
        snprintf(buffer, sizeof(buffer) - 1, "%s", linha);
    }
    
    IupSetAttribute(txt_resultado, "VALUE", buffer);
}

// Callback para popular combobox de clientes
static int popular_clientes(Ihandle *list) {
    Cliente clientes[1024];
    int total = pers_carregar_clientes(clientes, 1024);
    
    char item_text[200];
    for (int i = 0; i < total; i++) {
        snprintf(item_text, sizeof(item_text) - 1, "%d - %s", clientes[i].id, clientes[i].nome);
        IupSetStrAttribute(list, "APPENDITEM", item_text);
    }
    
    return IUP_DEFAULT;
}

// Callback para popular combobox de fornecedores
static int popular_fornecedores(Ihandle *list) {
    Fornecedor fornecedores[1024];
    int total = pers_carregar_fornecedores(fornecedores, 1024);
    
    char item_text[200];
    for (int i = 0; i < total; i++) {
        snprintf(item_text, sizeof(item_text) - 1, "%d - %s", fornecedores[i].id, fornecedores[i].nome_fantasia);
        IupSetStrAttribute(list, "APPENDITEM", item_text);
    }
    
    return IUP_DEFAULT;
}

// Callback para popular combobox de recursos
static int popular_recursos(Ihandle *list) {
    Recurso recursos[1024];
    int total = pers_carregar_recursos(recursos, 1024);
    
    char item_text[200];
    for (int i = 0; i < total; i++) {
        snprintf(item_text, sizeof(item_text) - 1, "%d - %s", recursos[i].id, recursos[i].descricao);
        IupSetStrAttribute(list, "APPENDITEM", item_text);
    }
    
    return IUP_DEFAULT;
}


static int exibir_relatorio_clientes(void) {
    limpar_resultado();
    
    FiltroCliente filtro;
    filtro.codigo_min = atoi(IupGetAttribute(txt_cod_min, "VALUE") ?: "0");
    filtro.codigo_max = atoi(IupGetAttribute(txt_cod_max, "VALUE") ?: "999999");
    filtro.ordem_alfabetica = strcmp(IupGetAttribute(chk_ordem_alfa, "VALUE"), "ON") == 0 ? 1 : 0;
    
    Cliente buffer[1024];
    int total = feedback_relatorio_clientes(buffer, 1024, filtro);
    
    // Cabeçalho
    char linha[500];
    snprintf(linha, sizeof(linha) - 1, "%-5s | %-30s | %-40s | %-20s", "Cód.", "Nome", "Email", "Telefone");
    adicionar_linha_resultado(linha);
    adicionar_linha_resultado("------|--------------------------------|------------------------------------------|--------------------");
    
    // Dados
    for (int i = 0; i < total; i++) {
        snprintf(linha, sizeof(linha) - 1, "%-5d | %-30s | %-40s | %-20s",
            buffer[i].id,
            buffer[i].nome,
            buffer[i].email,
            buffer[i].telefone);
        adicionar_linha_resultado(linha);
    }
    
    snprintf(linha, sizeof(linha) - 1, "\nTotal de registros: %d", total);
    adicionar_linha_resultado(linha);
    
    return IUP_DEFAULT;
}

static int exibir_relatorio_eventos(void) {
    limpar_resultado();
    
    FiltroEvento filtro;
    filtro.cliente_id = -1;
    filtro.data_min[0] = '\0';
    filtro.data_max[0] = '\0';
    filtro.status = -1;
    
    // Extrair filtros
    const char *sel_cliente = IupGetAttribute(list_clientes, "VALUE");
    if (sel_cliente && strlen(sel_cliente) > 0) {
        sscanf(sel_cliente, "%d", &filtro.cliente_id);
    }
    
    const char *data_min = IupGetAttribute(txt_data_min, "VALUE");
    if (data_min && strlen(data_min) > 0) {
        strcpy(filtro.data_min, data_min);
    }
    
    const char *data_max = IupGetAttribute(txt_data_max, "VALUE");
    if (data_max && strlen(data_max) > 0) {
        strcpy(filtro.data_max, data_max);
    }
    
    const char *sel_status = IupGetAttribute(list_status_evento, "VALUE");
    if (sel_status && strlen(sel_status) > 0) {
        int status_id = atoi(sel_status);
        if (status_id > 0) filtro.status = status_id;
    }
    
    Evento buffer[1024];
    int total = feedback_relatorio_eventos(buffer, 1024, filtro);
    
    char linha[600];
    snprintf(linha, sizeof(linha) - 1, "%-5s | %-30s | %-20s | %-15s | %-15s", 
        "Cód.", "Nome", "Período", "Status", "Total Estimado");
    adicionar_linha_resultado(linha);
    adicionar_linha_resultado("------|--------------------------------|----------------------|-----------------|---------------");
    
    for (int i = 0; i < total; i++) {
        const char *status_str = "Desconhecido";
        if (buffer[i].status == EVENTO_STATUS_ORCAMENTO) status_str = "Orçamento";
        else if (buffer[i].status == EVENTO_STATUS_APROVADO) status_str = "Aprovado";
        else if (buffer[i].status == EVENTO_STATUS_FINALIZADO) status_str = "Finalizado";
        
        snprintf(linha, sizeof(linha) - 1, "%-5d | %-30s | %s a %s | %-15s | R$ %.2f",
            buffer[i].id,
            buffer[i].nome,
            buffer[i].data_inicio,
            buffer[i].data_fim,
            status_str,
            buffer[i].total_estimado);
        adicionar_linha_resultado(linha);
    }
    
    snprintf(linha, sizeof(linha) - 1, "\nTotal de registros: %d", total);
    adicionar_linha_resultado(linha);
    
    return IUP_DEFAULT;
}

static int exibir_relatorio_equipamentos(void) {
    limpar_resultado();
    
    FiltroEquipamento filtro;
    filtro.codigo_min = atoi(IupGetAttribute(txt_cod_min, "VALUE") ?: "0");
    filtro.codigo_max = atoi(IupGetAttribute(txt_cod_max, "VALUE") ?: "999999");
    strcpy(filtro.categoria, IupGetAttribute(txt_cat_equip, "VALUE") ?: "");
    
    Recurso buffer[1024];
    int total = feedback_relatorio_equipamentos(buffer, 1024, filtro);
    
    char linha[600];
    snprintf(linha, sizeof(linha) - 1, "%-5s | %-25s | %-20s | %-8s | %-12s | %-12s",
        "Cód.", "Descrição", "Categoria", "Qtde", "Custo Unit.", "Locação/dia");
    adicionar_linha_resultado(linha);
    adicionar_linha_resultado("------|---------------------------|----------------------|----------|--|------------|--------");
    
    for (int i = 0; i < total; i++) {
        snprintf(linha, sizeof(linha) - 1, "%-5d | %-25s | %-20s | %-8d | R$ %9.2f | R$ %8.2f",
            buffer[i].id,
            buffer[i].descricao,
            buffer[i].categoria,
            buffer[i].quantidade,
            buffer[i].preco_custo,
            buffer[i].valor_locacao);
        adicionar_linha_resultado(linha);
    }
    
    snprintf(linha, sizeof(linha) - 1, "\nTotal de registros: %d", total);
    adicionar_linha_resultado(linha);
    
    return IUP_DEFAULT;
}

static int exibir_relatorio_contas_receber(void) {
    limpar_resultado();
    
    FiltroContasReceber filtro;
    filtro.cliente_id = -1;
    filtro.data_min[0] = '\0';
    filtro.data_max[0] = '\0';
    
    const char *sel_cliente = IupGetAttribute(list_clientes_cr, "VALUE");
    if (sel_cliente && strlen(sel_cliente) > 0) {
        sscanf(sel_cliente, "%d", &filtro.cliente_id);
    }
    
    const char *data_min = IupGetAttribute(txt_data_min_cr, "VALUE");
    if (data_min && strlen(data_min) > 0) {
        strcpy(filtro.data_min, data_min);
    }
    
    const char *data_max = IupGetAttribute(txt_data_max_cr, "VALUE");
    if (data_max && strlen(data_max) > 0) {
        strcpy(filtro.data_max, data_max);
    }
    
    ContaReceber buffer[1024];
    int total = feedback_relatorio_contas_receber(buffer, 1024, filtro);
    
    char linha[600];
    snprintf(linha, sizeof(linha) - 1, "%-5s | %-8s | %-15s | %-15s | %-12s | %-12s | %-10s",
        "Cód.", "Evento", "Data Emissão", "Data Venc.", "Fatura", "Recebido", "Status");
    adicionar_linha_resultado(linha);
    adicionar_linha_resultado("------|----------|-----------------|-----------------|------------|--|----------|");
    
    for (int i = 0; i < total; i++) {
        const char *status_str = "Aberto";
        if (buffer[i].status == 1) status_str = "Parcial";
        else if (buffer[i].status == 2) status_str = "Liquidado";
        
        snprintf(linha, sizeof(linha) - 1, "%-5d | %-8d | %15s | %15s | R$ %9.2f | R$ %7.2f | %-10s",
            buffer[i].id,
            buffer[i].evento_id,
            buffer[i].data_emissao,
            buffer[i].data_vencimento,
            buffer[i].valor_fatura,
            buffer[i].valor_recebido,
            status_str);
        adicionar_linha_resultado(linha);
    }
    
    snprintf(linha, sizeof(linha) - 1, "\nTotal de registros: %d", total);
    adicionar_linha_resultado(linha);
    
    return IUP_DEFAULT;
}

static int exibir_relatorio_contas_pagar(void) {
    limpar_resultado();
    
    FiltroContasPagar filtro;
    filtro.fornecedor_id = -1;
    filtro.data_min[0] = '\0';
    filtro.data_max[0] = '\0';
    
    const char *sel_forn = IupGetAttribute(list_fornecedores, "VALUE");
    if (sel_forn && strlen(sel_forn) > 0) {
        sscanf(sel_forn, "%d", &filtro.fornecedor_id);
    }
    
    const char *data_min = IupGetAttribute(txt_data_min_cp, "VALUE");
    if (data_min && strlen(data_min) > 0) {
        strcpy(filtro.data_min, data_min);
    }
    
    const char *data_max = IupGetAttribute(txt_data_max_cp, "VALUE");
    if (data_max && strlen(data_max) > 0) {
        strcpy(filtro.data_max, data_max);
    }
    
    ContaPagar buffer[1024];
    int total = feedback_relatorio_contas_pagar(buffer, 1024, filtro);
    
    char linha[600];
    snprintf(linha, sizeof(linha) - 1, "%-5s | %-20s | %-8s | %-15s | %-12s | %-8s",
        "Cód.", "Fornecedor", "Compra", "Data Venc.", "Valor", "Status");
    adicionar_linha_resultado(linha);
    adicionar_linha_resultado("------|--------|----------|-----------------|----------|");
    
    for (int i = 0; i < total; i++) {
        char forn_nome[120];
        feedback_obter_fornecedor_nome(buffer[i].fornecedor_id, forn_nome, sizeof(forn_nome));
        
        const char *status_str = (buffer[i].status == 0) ? "Aberto" : "Pago";
        
        snprintf(linha, sizeof(linha) - 1, "%-5d | %-20s | %-8d | %15s | R$ %9.2f | %-8s",
            buffer[i].id,
            forn_nome,
            buffer[i].compra_id,
            buffer[i].data_vencimento,
            buffer[i].valor,
            status_str);
        adicionar_linha_resultado(linha);
    }
    
    snprintf(linha, sizeof(linha) - 1, "\nTotal de registros: %d", total);
    adicionar_linha_resultado(linha);
    
    return IUP_DEFAULT;
}

static int exibir_relatorio_caixa(void) {
    limpar_resultado();
    
    FiltroCaixa filtro;
    strcpy(filtro.data_min, IupGetAttribute(txt_data_min_caixa, "VALUE") ?: "");
    strcpy(filtro.data_max, IupGetAttribute(txt_data_max_caixa, "VALUE") ?: "");
    
    CaixaLancamento buffer[1024];
    int total = feedback_relatorio_caixa(buffer, 1024, filtro);
    
    char linha[600];
    snprintf(linha, sizeof(linha) - 1, "%-5s | %-15s | %-10s | %-10s | %-12s | %-30s",
        "Cód.", "Data", "Tipo", "Forma", "Valor", "Descrição");
    adicionar_linha_resultado(linha);
    adicionar_linha_resultado("------|-----------------|----------|----------|----------|");
    
    for (int i = 0; i < total; i++) {
        snprintf(linha, sizeof(linha) - 1, "%-5d | %15s | %-10s | %-10s | R$ %9.2f | %-30s",
            buffer[i].id,
            buffer[i].data,
            buffer[i].tipo,
            buffer[i].forma,
            buffer[i].valor,
            buffer[i].descricao);
        adicionar_linha_resultado(linha);
    }
    
    snprintf(linha, sizeof(linha) - 1, "\nTotal de registros: %d", total);
    adicionar_linha_resultado(linha);
    
    return IUP_DEFAULT;
}

/* ========================================================================
 * CALLBACKS DE BOTÕES
 * ======================================================================== */

static int btn_gerar_relatorio_cb(Ihandle *self) {
    const char *sel_relatorio = IupGetAttribute(list_relatorios, "VALUE");
    if (!sel_relatorio || strlen(sel_relatorio) == 0) {
        IupMessage("Aviso", "Selecione um relatório!");
        return IUP_DEFAULT;
    }
    
    int tipo = atoi(sel_relatorio);
    
    switch (tipo) {
        case 1: exibir_relatorio_clientes(); break;
        case 2: exibir_relatorio_eventos(); break;
        case 3: exibir_relatorio_equipamentos(); break;
        case 4: exibir_relatorio_contas_receber(); break;
        case 5: exibir_relatorio_contas_pagar(); break;
        case 6: exibir_relatorio_caixa(); break;
        default:
            IupMessage("Erro", "Tipo de relatório inválido!");
    }
    
    return IUP_DEFAULT;
}

static int btn_exportar_csv_cb(Ihandle *self) {
    const char *sel_relatorio = IupGetAttribute(list_relatorios, "VALUE");
    if (!sel_relatorio || strlen(sel_relatorio) == 0) {
        IupMessage("Aviso", "Selecione um relatório!");
        return IUP_DEFAULT;
    }
    
    // Diálogo para escolher caminho do arquivo
    Ihandle *filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
    IupSetAttribute(filedlg, "EXTFILTER", "CSV files|*.csv|All files|*.*");
    IupSetAttribute(filedlg, "FILE", "relatorio.csv");
    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);
    
    int status = IupGetInt(filedlg, "STATUS");
    if (status != -1) {
        const char *caminho = IupGetAttribute(filedlg, "VALUE");
        
        int tipo = atoi(sel_relatorio);
        int sucesso = 0;
        
        switch (tipo) {
            case 1: {
                FiltroCliente filtro;
                filtro.codigo_min = atoi(IupGetAttribute(txt_cod_min, "VALUE") ?: "0");
                filtro.codigo_max = atoi(IupGetAttribute(txt_cod_max, "VALUE") ?: "999999");
                filtro.ordem_alfabetica = strcmp(IupGetAttribute(chk_ordem_alfa, "VALUE"), "ON") == 0 ? 1 : 0;
                sucesso = feedback_exportar_clientes_csv(caminho, filtro);
                break;
            }
            case 2: {
                FiltroEvento filtro;
                filtro.cliente_id = -1;
                filtro.data_min[0] = '\0';
                filtro.data_max[0] = '\0';
                filtro.status = -1;
                
                const char *sel = IupGetAttribute(list_clientes, "VALUE");
                if (sel && strlen(sel) > 0) sscanf(sel, "%d", &filtro.cliente_id);
                const char *dm = IupGetAttribute(txt_data_min, "VALUE");
                if (dm) strcpy(filtro.data_min, dm);
                const char *dx = IupGetAttribute(txt_data_max, "VALUE");
                if (dx) strcpy(filtro.data_max, dx);
                const char *st = IupGetAttribute(list_status_evento, "VALUE");
                if (st && strlen(st) > 0) filtro.status = atoi(st);
                
                sucesso = feedback_exportar_eventos_csv(caminho, filtro);
                break;
            }
            case 3: {
                FiltroEquipamento filtro;
                filtro.codigo_min = atoi(IupGetAttribute(txt_cod_min, "VALUE") ?: "0");
                filtro.codigo_max = atoi(IupGetAttribute(txt_cod_max, "VALUE") ?: "999999");
                strcpy(filtro.categoria, IupGetAttribute(txt_cat_equip, "VALUE") ?: "");
                sucesso = feedback_exportar_equipamentos_csv(caminho, filtro);
                break;
            }
            case 4: {
                FiltroContasReceber filtro;
                filtro.cliente_id = -1;
                filtro.data_min[0] = '\0';
                filtro.data_max[0] = '\0';
                
                const char *sel = IupGetAttribute(list_clientes_cr, "VALUE");
                if (sel && strlen(sel) > 0) sscanf(sel, "%d", &filtro.cliente_id);
                const char *dm = IupGetAttribute(txt_data_min_cr, "VALUE");
                if (dm) strcpy(filtro.data_min, dm);
                const char *dx = IupGetAttribute(txt_data_max_cr, "VALUE");
                if (dx) strcpy(filtro.data_max, dx);
                
                sucesso = feedback_exportar_contas_receber_csv(caminho, filtro);
                break;
            }
            case 5: {
                FiltroContasPagar filtro;
                filtro.fornecedor_id = -1;
                filtro.data_min[0] = '\0';
                filtro.data_max[0] = '\0';
                
                const char *sel = IupGetAttribute(list_fornecedores, "VALUE");
                if (sel && strlen(sel) > 0) sscanf(sel, "%d", &filtro.fornecedor_id);
                const char *dm = IupGetAttribute(txt_data_min_cp, "VALUE");
                if (dm) strcpy(filtro.data_min, dm);
                const char *dx = IupGetAttribute(txt_data_max_cp, "VALUE");
                if (dx) strcpy(filtro.data_max, dx);
                
                sucesso = feedback_exportar_contas_pagar_csv(caminho, filtro);
                break;
            }
            case 6: {
                FiltroCaixa filtro;
                strcpy(filtro.data_min, IupGetAttribute(txt_data_min_caixa, "VALUE") ?: "");
                strcpy(filtro.data_max, IupGetAttribute(txt_data_max_caixa, "VALUE") ?: "");
                sucesso = feedback_exportar_caixa_csv(caminho, filtro);
                break;
            }
        }
        
        if (sucesso) {
            IupMessage("Sucesso", "Relatório exportado com sucesso!");
        } else {
            IupMessage("Erro", "Erro ao exportar relatório!");
        }
    }
    
    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

static int list_relatorios_cb(Ihandle *self, const char *text, int item, int state) {
    if (state == 1) {
        // Limpar e popular filtros conforme tipo selecionado
        if (box_filtros) IupDestroy(box_filtros);
        
        Ihandle *lbl_info = IupLabel("Nenhum filtro disponível");
        
        int tipo = item;
        
        switch (tipo) {
            case 1: {
                // Clientes: código min/max e ordem
                txt_cod_min = IupText(NULL);
                IupSetAttribute(txt_cod_min, "VALUE", "1");
                IupSetAttribute(txt_cod_min, "READONLY", "NO");
                
                txt_cod_max = IupText(NULL);
                IupSetAttribute(txt_cod_max, "VALUE", "999999");
                IupSetAttribute(txt_cod_max, "READONLY", "NO");
                
                chk_ordem_alfa = IupToggle("Ordenar alfabeticamente", NULL);
                IupSetAttribute(chk_ordem_alfa, "VALUE", "OFF");
                
                box_filtros = IupVbox(
                    IupLabel("Código Mínimo:"),
                    txt_cod_min,
                    IupLabel("Código Máximo:"),
                    txt_cod_max,
                    chk_ordem_alfa,
                    NULL
                );
                break;
            }
            case 2: {
                // Eventos: cliente, período, status
                list_clientes = IupList(NULL);
                IupSetAttribute(list_clientes, "EXPAND", "YES");
                IupSetAttribute(list_clientes, "SIZE0", "200");
                IupSetAttribute(list_clientes, "VISIBLE_ITEMS", "5");
                popular_clientes(list_clientes);
                
                txt_data_min = IupText(NULL);
                IupSetAttribute(txt_data_min, "VALUE", "");
                
                txt_data_max = IupText(NULL);
                IupSetAttribute(txt_data_max, "VALUE", "");
                
                list_status_evento = IupList(NULL);
                IupSetStrAttribute(list_status_evento, "APPENDITEM", "Todos");
                IupSetStrAttribute(list_status_evento, "APPENDITEM", "1|Orçamento");
                IupSetStrAttribute(list_status_evento, "APPENDITEM", "2|Aprovado");
                IupSetStrAttribute(list_status_evento, "APPENDITEM", "3|Finalizado");
                IupSetAttribute(list_status_evento, "SIZE0", "200");
                IupSetAttribute(list_status_evento, "VISIBLE_ITEMS", "4");
                
                box_filtros = IupVbox(
                    IupLabel("Cliente:"),
                    IupFrame(list_clientes),
                    IupLabel("Data Início (YYYY-MM-DD):"),
                    txt_data_min,
                    IupLabel("Data Fim (YYYY-MM-DD):"),
                    txt_data_max,
                    IupLabel("Status:"),
                    IupFrame(list_status_evento),
                    NULL
                );
                break;
            }
            case 3: {
                // Equipamentos: código, categoria
                txt_cod_min = IupText(NULL);
                IupSetAttribute(txt_cod_min, "VALUE", "1");
                
                txt_cod_max = IupText(NULL);
                IupSetAttribute(txt_cod_max, "VALUE", "999999");
                
                txt_cat_equip = IupText(NULL);
                IupSetAttribute(txt_cat_equip, "VALUE", "");
                
                box_filtros = IupVbox(
                    IupLabel("Código Mínimo:"),
                    txt_cod_min,
                    IupLabel("Código Máximo:"),
                    txt_cod_max,
                    IupLabel("Categoria (em branco = todas):"),
                    txt_cat_equip,
                    NULL
                );
                break;
            }
            case 4: {
                // Contas Receber: cliente, período
                list_clientes_cr = IupList(NULL);
                IupSetAttribute(list_clientes_cr, "SIZE0", "200");
                IupSetAttribute(list_clientes_cr, "VISIBLE_ITEMS", "5");
                popular_clientes(list_clientes_cr);
                
                txt_data_min_cr = IupText(NULL);
                txt_data_max_cr = IupText(NULL);
                
                box_filtros = IupVbox(
                    IupLabel("Cliente:"),
                    IupFrame(list_clientes_cr),
                    IupLabel("Data Vencimento Mínima:"),
                    txt_data_min_cr,
                    IupLabel("Data Vencimento Máxima:"),
                    txt_data_max_cr,
                    NULL
                );
                break;
            }
            case 5: {
                // Contas Pagar: fornecedor, período
                list_fornecedores = IupList(NULL);
                IupSetAttribute(list_fornecedores, "SIZE0", "200");
                IupSetAttribute(list_fornecedores, "VISIBLE_ITEMS", "5");
                popular_fornecedores(list_fornecedores);
                
                txt_data_min_cp = IupText(NULL);
                txt_data_max_cp = IupText(NULL);
                
                box_filtros = IupVbox(
                    IupLabel("Fornecedor:"),
                    IupFrame(list_fornecedores),
                    IupLabel("Data Vencimento Mínima:"),
                    txt_data_min_cp,
                    IupLabel("Data Vencimento Máxima:"),
                    txt_data_max_cp,
                    NULL
                );
                break;
            }
            case 6: {
                // Caixa: período
                txt_data_min_caixa = IupText(NULL);
                txt_data_max_caixa = IupText(NULL);
                
                box_filtros = IupVbox(
                    IupLabel("Data Início (YYYY-MM-DD):"),
                    txt_data_min_caixa,
                    IupLabel("Data Fim (YYYY-MM-DD):"),
                    txt_data_max_caixa,
                    NULL
                );
                break;
            }
        }
        
        if (box_filtros) {
            IupSetAttribute(box_filtros, "GAP", "5");
            IupSetAttribute(box_filtros, "MARGIN", "5x5");
            // Atualizar a janela dinamicamente
            IupRefresh(dlg_feedback);
        }
    }
    
    return IUP_DEFAULT;
}


void feedback_view_mostrar(void) {
    if (dlg_feedback) {
        IupShowXY(dlg_feedback, IUP_CENTER, IUP_CENTER);
        return;
    }
    
    // Lista de relatórios
    list_relatorios = IupList(NULL);
    IupSetStrAttribute(list_relatorios, "APPENDITEM", "1|Listagem de Clientes");
    IupSetStrAttribute(list_relatorios, "APPENDITEM", "2|Listagem de Eventos");
    IupSetStrAttribute(list_relatorios, "APPENDITEM", "3|Listagem de Equipamentos");
    IupSetStrAttribute(list_relatorios, "APPENDITEM", "4|Contas a Receber");
    IupSetStrAttribute(list_relatorios, "APPENDITEM", "5|Contas a Pagar");
    IupSetStrAttribute(list_relatorios, "APPENDITEM", "6|Movimentação de Caixa");
    IupSetAttribute(list_relatorios, "SIZE0", "250");
    IupSetAttribute(list_relatorios, "VISIBLE_ITEMS", "6");
    IupSetAttribute(list_relatorios, "EXPAND", "YES");
    IupSetCallback(list_relatorios, "ACTION", (Icallback)list_relatorios_cb);
    
    // Área para filtros (inicialmente vazia)
    box_filtros = IupVbox(
        IupLabel("Selecione um relatório para ver os filtros disponíveis"),
        NULL
    );
    IupSetAttribute(box_filtros, "GAP", "5");
    IupSetAttribute(box_filtros, "MARGIN", "5x5");
    
    // Área de resultado (texto)
    txt_resultado = IupText(NULL);
    IupSetAttribute(txt_resultado, "MULTILINE", "YES");
    IupSetAttribute(txt_resultado, "EXPAND", "YES");
    IupSetAttribute(txt_resultado, "READONLY", "YES");
    IupSetAttribute(txt_resultado, "VALUE", "Selecione um relatório e clique em 'Gerar' para visualizar");
    
    // Botões de ação
    Ihandle *btn_gerar = IupButton("Gerar Relatório", NULL);
    IupSetCallback(btn_gerar, "K_ANY", (Icallback)btn_gerar_relatorio_cb);
    
    Ihandle *btn_exportar = IupButton("Exportar para CSV", NULL);
    IupSetCallback(btn_exportar, "K_ANY", (Icallback)btn_exportar_csv_cb);
    
    Ihandle *btn_fechar = IupButton("Fechar", NULL);
    IupSetCallback(btn_fechar, "K_ANY", (Icallback)IupHide);
    
    Ihandle *hbox_botoes = IupHbox(btn_gerar, btn_exportar, btn_fechar, NULL);
    IupSetAttribute(hbox_botoes, "GAP", "10");
    IupSetAttribute(hbox_botoes, "MARGIN", "5x5");
    
    // Layout principal
    Ihandle *hbox_principal = IupHbox(
        IupVbox(
            IupLabel("Relatórios:"),
            IupFrame(list_relatorios),
            NULL
        ),
        IupVbox(
            IupLabel("Filtros:"),
            IupFrame(box_filtros),
            NULL
        ),
        NULL
    );
    IupSetAttribute(hbox_principal, "GAP", "10");
    IupSetAttribute(hbox_principal, "MARGIN", "5x5");
    
    Ihandle *vbox_resultado = IupVbox(
        IupLabel("Resultado:"),
        IupFrame(txt_resultado),
        NULL
    );
    IupSetAttribute(vbox_resultado, "GAP", "5");
    IupSetAttribute(vbox_resultado, "MARGIN", "5x5");
    
    Ihandle *vbox_main = IupVbox(
        hbox_principal,
        vbox_resultado,
        hbox_botoes,
        NULL
    );
    IupSetAttribute(vbox_main, "GAP", "5");
    
    dlg_feedback = IupDialog(vbox_main);
    IupSetAttribute(dlg_feedback, "TITLE", "Módulo de Feedback - Relatórios");
    IupSetAttribute(dlg_feedback, "SIZE", "900x600");
    IupSetAttribute(dlg_feedback, "RESIZE", "YES");
    
    IupShowXY(dlg_feedback, IUP_CENTER, IUP_CENTER);
}
