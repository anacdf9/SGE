

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iup.h>
#include <iupcontrols.h>
#include "../controller/trade_controller.h"



static Ihandle *dlg_trade = NULL;

// Checkboxes de tabelas
static Ihandle *chk_clientes = NULL;
static Ihandle *chk_equipamentos = NULL;
static Ihandle *chk_eventos = NULL;
static Ihandle *chk_contas_receber = NULL;
static Ihandle *chk_contas_pagar = NULL;
static Ihandle *chk_caixa = NULL;
static Ihandle *chk_fornecedores = NULL;
static Ihandle *chk_operadores = NULL;
static Ihandle *chk_equipes = NULL;
static Ihandle *chk_produtora = NULL;

// Rádio buttons e campos
static Ihandle *rdb_exportar = NULL;
static Ihandle *rdb_importar = NULL;
static Ihandle *chk_sobrescrever = NULL;
static Ihandle *txt_resultado = NULL;
static Ihandle *rdb_formato_xml = NULL;
static Ihandle *rdb_formato_csv = NULL;



static int obter_tabelas_selecionadas(void) {
    int tabelas = 0;
    
    if (strcmp(IupGetAttribute(chk_clientes, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_CLIENTES;
    if (strcmp(IupGetAttribute(chk_equipamentos, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_EQUIPAMENTOS;
    if (strcmp(IupGetAttribute(chk_eventos, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_EVENTOS;
    if (strcmp(IupGetAttribute(chk_contas_receber, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_CONTAS_RECEBER;
    if (strcmp(IupGetAttribute(chk_contas_pagar, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_CONTAS_PAGAR;
    if (strcmp(IupGetAttribute(chk_caixa, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_CAIXA;
    if (strcmp(IupGetAttribute(chk_fornecedores, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_FORNECEDORES;
    if (strcmp(IupGetAttribute(chk_operadores, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_OPERADORES;
    if (strcmp(IupGetAttribute(chk_equipes, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_EQUIPES;
    if (strcmp(IupGetAttribute(chk_produtora, "VALUE"), "ON") == 0)
        tabelas |= TRADE_TABLE_PRODUTORA;
    
    return tabelas;
}

static void adicionar_resultado(const char *texto) {
    if (!txt_resultado || !texto) return;
    
    const char *atual = IupGetAttribute(txt_resultado, "VALUE");
    char buffer[8192];
    
    if (atual && strlen(atual) > 0) {
        snprintf(buffer, sizeof(buffer) - 1, "%s\n%s", atual, texto);
    } else {
        snprintf(buffer, sizeof(buffer) - 1, "%s", texto);
    }
    
    IupSetAttribute(txt_resultado, "VALUE", buffer);
}

static void selecionar_todos(void) {
    IupSetAttribute(chk_clientes, "VALUE", "ON");
    IupSetAttribute(chk_equipamentos, "VALUE", "ON");
    IupSetAttribute(chk_eventos, "VALUE", "ON");
    IupSetAttribute(chk_contas_receber, "VALUE", "ON");
    IupSetAttribute(chk_contas_pagar, "VALUE", "ON");
    IupSetAttribute(chk_caixa, "VALUE", "ON");
    IupSetAttribute(chk_fornecedores, "VALUE", "ON");
    IupSetAttribute(chk_operadores, "VALUE", "ON");
    IupSetAttribute(chk_equipes, "VALUE", "ON");
    IupSetAttribute(chk_produtora, "VALUE", "ON");
}

static void desselecionar_todos(void) {
    IupSetAttribute(chk_clientes, "VALUE", "OFF");
    IupSetAttribute(chk_equipamentos, "VALUE", "OFF");
    IupSetAttribute(chk_eventos, "VALUE", "OFF");
    IupSetAttribute(chk_contas_receber, "VALUE", "OFF");
    IupSetAttribute(chk_contas_pagar, "VALUE", "OFF");
    IupSetAttribute(chk_caixa, "VALUE", "OFF");
    IupSetAttribute(chk_fornecedores, "VALUE", "OFF");
    IupSetAttribute(chk_operadores, "VALUE", "OFF");
    IupSetAttribute(chk_equipes, "VALUE", "OFF");
    IupSetAttribute(chk_produtora, "VALUE", "OFF");
}



static int btn_selecionar_todos_cb(Ihandle *self) {
    selecionar_todos();
    return IUP_DEFAULT;
}

static int btn_desselecionar_todos_cb(Ihandle *self) {
    desselecionar_todos();
    return IUP_DEFAULT;
}

static int btn_exportar_cb(Ihandle *self) {
    int tabelas = obter_tabelas_selecionadas();
    if (tabelas == 0) {
        IupMessage("Aviso", "Selecione ao menos uma tabela para exportar!");
        return IUP_DEFAULT;
    }
    
    // Determina formato
    int usar_csv = (rdb_formato_csv && strcmp(IupGetAttribute(rdb_formato_csv, "VALUE"), "ON") == 0) ? 1 : 0;
    
    // Diálogo para escolher arquivo
    Ihandle *filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
    
    if (usar_csv) {
        IupSetAttribute(filedlg, "EXTFILTER", "CSV files|*.csv|All files|*.*");
        IupSetAttribute(filedlg, "FILE", "dados.csv");
    } else {
        IupSetAttribute(filedlg, "EXTFILTER", "XML files|*.xml|All files|*.*");
        IupSetAttribute(filedlg, "FILE", "dados.xml");
    }
    
    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);
    
    int status = IupGetInt(filedlg, "STATUS");
    if (status != -1) {
        const char *caminho = IupGetAttribute(filedlg, "VALUE");
        
        IupSetAttribute(txt_resultado, "VALUE", "Exportando dados...\n");
        
        int total;
        if (usar_csv) {
            total = trade_exportar_csv(caminho, tabelas, ',');
        } else {
            total = trade_exportar_xml(caminho, tabelas);
        }
        
        if (total >= 0) {
            char msg[256];
            snprintf(msg, sizeof(msg), "✓ Exportação concluída!\n%d registros exportados para:\n%s", total, caminho);
            adicionar_resultado(msg);
            IupMessage("Sucesso", msg);
        } else {
            adicionar_resultado("✗ Erro ao exportar dados!");
            IupMessage("Erro", "Erro ao exportar dados!");
        }
    }
    
    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

static int btn_importar_cb(Ihandle *self) {
    int tabelas = obter_tabelas_selecionadas();
    if (tabelas == 0) {
        IupMessage("Aviso", "Selecione ao menos uma tabela para importar!");
        return IUP_DEFAULT;
    }
    
    // Determina formato
    int usar_csv = (rdb_formato_csv && strcmp(IupGetAttribute(rdb_formato_csv, "VALUE"), "ON") == 0) ? 1 : 0;
    
    // Diálogo para escolher arquivo
    Ihandle *filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
    
    if (usar_csv) {
        IupSetAttribute(filedlg, "EXTFILTER", "CSV files|*.csv|All files|*.*");
    } else {
        IupSetAttribute(filedlg, "EXTFILTER", "XML files|*.xml|All files|*.*");
    }
    
    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);
    
    int status = IupGetInt(filedlg, "STATUS");
    if (status != -1) {
        const char *caminho = IupGetAttribute(filedlg, "VALUE");
        
        IupSetAttribute(txt_resultado, "VALUE", "Importando dados...\n");
        
        int sobrescrever = strcmp(IupGetAttribute(chk_sobrescrever, "VALUE"), "ON") == 0 ? 1 : 0;
        
        if (sobrescrever) {
            int resposta = IupAlarm(
                "Confirmação",
                "Isso sobrescrevará os dados existentes!\nDeseja continuar?",
                "Sim", "Não"
            );
            if (resposta == 2) {
                IupDestroy(filedlg);
                return IUP_DEFAULT;
            }
        }
        
        int total;
        if (usar_csv) {
            total = trade_importar_csv(caminho, tabelas, ',', sobrescrever);
        } else {
            total = trade_importar_xml(caminho, tabelas, sobrescrever);
        }
        
        if (total >= 0) {
            char msg[256];
            snprintf(msg, sizeof(msg), "✓ Importação concluída!\n%d registros importados de:\n%s", total, caminho);
            adicionar_resultado(msg);
            IupMessage("Sucesso", msg);
        } else {
            adicionar_resultado("✗ Erro ao importar dados!");
            IupMessage("Erro", "Erro ao abrir arquivo de importação!");
        }
    }
    
    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

static int btn_limpar_cb(Ihandle *self) {
    IupSetAttribute(txt_resultado, "VALUE", "");
    return IUP_DEFAULT;
}



void trade_view_mostrar(void) {
    if (dlg_trade) {
        IupShowXY(dlg_trade, IUP_CENTER, IUP_CENTER);
        return;
    }
    
    // ========== SELEÇÃO DE TABELAS ==========
    
    Ihandle *lbl_tabelas = IupLabel("Selecione as tabelas:");
    
    chk_clientes = IupToggle("Clientes", NULL);
    chk_equipamentos = IupToggle("Equipamentos", NULL);
    chk_eventos = IupToggle("Eventos", NULL);
    chk_contas_receber = IupToggle("Contas a Receber", NULL);
    chk_contas_pagar = IupToggle("Contas a Pagar", NULL);
    chk_caixa = IupToggle("Movimentação de Caixa", NULL);
    chk_fornecedores = IupToggle("Fornecedores", NULL);
    chk_operadores = IupToggle("Operadores", NULL);
    chk_equipes = IupToggle("Equipes", NULL);
    chk_produtora = IupToggle("Produtora", NULL);
    
    IupSetAttribute(chk_clientes, "VALUE", "ON");
    IupSetAttribute(chk_equipamentos, "VALUE", "ON");
    IupSetAttribute(chk_eventos, "VALUE", "ON");
    
    Ihandle *btn_selecionar_todos = IupButton("Selecionar Tudo", NULL);
    IupSetCallback(btn_selecionar_todos, "K_ANY", (Icallback)btn_selecionar_todos_cb);
    
    Ihandle *btn_desselecionar_todos = IupButton("Desselecionar Tudo", NULL);
    IupSetCallback(btn_desselecionar_todos, "K_ANY", (Icallback)btn_desselecionar_todos_cb);
    
    Ihandle *hbox_botoes_tabelas = IupHbox(btn_selecionar_todos, btn_desselecionar_todos, NULL);
    IupSetAttribute(hbox_botoes_tabelas, "GAP", "5");
    
    Ihandle *vbox_tabelas = IupVbox(
        lbl_tabelas,
        chk_clientes,
        chk_equipamentos,
        chk_eventos,
        chk_contas_receber,
        chk_contas_pagar,
        chk_caixa,
        chk_fornecedores,
        chk_operadores,
        chk_equipes,
        chk_produtora,
        hbox_botoes_tabelas,
        NULL
    );
    IupSetAttribute(vbox_tabelas, "GAP", "3");
    IupSetAttribute(vbox_tabelas, "MARGIN", "5x5");
    
    // ========== OPÇÕES ==========
    
    // Formato de arquivo
    Ihandle *rb_formato = IupRadio(IupVbox(
        rdb_formato_xml = IupToggle("XML (padrão)", NULL),
        rdb_formato_csv = IupToggle("CSV (delimitado por vírgula)", NULL),
        NULL
    ));
    IupSetAttribute(rdb_formato_xml, "VALUE", "ON");
    
    chk_sobrescrever = IupToggle("Sobrescrever dados existentes na importação", NULL);
    IupSetAttribute(chk_sobrescrever, "VALUE", "OFF");
    
    Ihandle *vbox_opcoes = IupVbox(
        IupLabel("Formato:"),
        rb_formato,
        IupLabel("Opções:"),
        chk_sobrescrever,
        NULL
    );
    IupSetAttribute(vbox_opcoes, "GAP", "5");
    IupSetAttribute(vbox_opcoes, "MARGIN", "5x5");
    
    // ========== ÁREA DE RESULTADO ==========
    
    txt_resultado = IupText(NULL);
    IupSetAttribute(txt_resultado, "MULTILINE", "YES");
    IupSetAttribute(txt_resultado, "EXPAND", "YES");
    IupSetAttribute(txt_resultado, "READONLY", "YES");
    IupSetAttribute(txt_resultado, "VALUE", "Aguardando ação...");
    IupSetAttribute(txt_resultado, "VISIBLELINES", "10");
    
    Ihandle *vbox_resultado = IupVbox(
        IupLabel("Status:"),
        txt_resultado,
        NULL
    );
    IupSetAttribute(vbox_resultado, "GAP", "5");
    IupSetAttribute(vbox_resultado, "MARGIN", "5x5");
    
    // ========== BOTÕES DE AÇÃO ==========
    
    Ihandle *btn_exportar = IupButton("Exportar para XML", NULL);
    IupSetCallback(btn_exportar, "K_ANY", (Icallback)btn_exportar_cb);
    
    Ihandle *btn_importar = IupButton("Importar de XML", NULL);
    IupSetCallback(btn_importar, "K_ANY", (Icallback)btn_importar_cb);
    
    Ihandle *btn_limpar = IupButton("Limpar Status", NULL);
    IupSetCallback(btn_limpar, "K_ANY", (Icallback)btn_limpar_cb);
    
    Ihandle *btn_fechar = IupButton("Fechar", NULL);
    IupSetCallback(btn_fechar, "K_ANY", (Icallback)IupHide);
    
    Ihandle *hbox_botoes_acao = IupHbox(
        btn_exportar,
        btn_importar,
        btn_limpar,
        IupFill(),
        btn_fechar,
        NULL
    );
    IupSetAttribute(hbox_botoes_acao, "GAP", "10");
    IupSetAttribute(hbox_botoes_acao, "MARGIN", "5x5");
    
    // ========== LAYOUT PRINCIPAL ==========
    
    Ihandle *hbox_esquerda = IupVbox(
        IupFrame(vbox_tabelas),
        IupFrame(vbox_opcoes),
        NULL
    );
    IupSetAttribute(hbox_esquerda, "GAP", "10");
    
    Ihandle *hbox_principal = IupHbox(
        hbox_esquerda,
        IupFrame(vbox_resultado),
        NULL
    );
    IupSetAttribute(hbox_principal, "GAP", "10");
    IupSetAttribute(hbox_principal, "MARGIN", "10x10");
    
    Ihandle *vbox_main = IupVbox(
        hbox_principal,
        hbox_botoes_acao,
        NULL
    );
    
    dlg_trade = IupDialog(vbox_main);
    IupSetAttribute(dlg_trade, "TITLE", "Importação/Exportação de Dados");
    IupSetAttribute(dlg_trade, "SIZE", "800x600");
    IupSetAttribute(dlg_trade, "RESIZE", "YES");
    
    IupShowXY(dlg_trade, IUP_CENTER, IUP_CENTER);
}
