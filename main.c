/*
 * ========================================================================
 * SISTEMA DE GESTÃO DE EVENTOS (SGE)
 * ========================================================================
 * Descrição: Sistema completo para gerenciamento de eventos, incluindo
 *           cadastros, orçamentos e controle financeiro
 * 
 * Módulos:
 *   - Cadastros (Clientes, Equipes, Recursos, Fornecedores, etc)
 *   - Orçamentos e Eventos
 *   - Transações Financeiras
 * ========================================================================
 */

#include <iup.h>
#include <iupcontrols.h> 
#include <stdlib.h>  

/* Includes das Views (Interfaces) */
#include "src/view/cliente_view.h"
#include "src/view/equipe_view.h"
#include "src/view/recurso_view.h"
#include "src/view/fornecedor_view.h"
#include "src/view/operador_view.h"
#include "src/view/produtora_view.h"
#include "src/view/evento_view.h"
#include "src/view/transacoes_view.h"
#include "src/view/feedback_view.h"
#include "src/view/trade_view.h"

/* Includes de Configuração */
#include "src/config.h"
#include "src/model/pers.h"

/* ========================================================================
 * FUNÇÕES DE CALLBACK (AÇÕES DOS MENUS)
 * ======================================================================== */

/*
 * Abre a tela de cadastro de Clientes
 */
int btn_abrir_clientes_cb(void) {
    cliente_view_mostrar(); 
    return IUP_DEFAULT;
}

/*
 * Abre a tela de cadastro de Equipes
 */
int btn_abrir_equipe_cb(void) {
    equipe_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Abre a tela de cadastro de Recursos/Equipamentos
 */
int btn_abrir_recursos_cb(void) {
    recurso_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Abre a tela de cadastro de Fornecedores
 */
int btn_abrir_fornecedores_cb(void) {
    fornecedor_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Abre a tela de cadastro de Operadores do Sistema
 */
int btn_abrir_operadores_cb(void) {
    operador_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Abre a tela de cadastro da Produtora
 */
int btn_abrir_produtora_cb(void) {
    produtora_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Abre a tela de Orçamentos e Eventos
 */
int btn_abrir_eventos_cb(void) {
    evento_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Abre a tela de Transações Financeiras
 */
int btn_abrir_transacoes_cb(void) {
    transacoes_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Abre o módulo de Feedback com relatórios
 */
int btn_abrir_feedback_cb(void) {
    feedback_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Abre o módulo de Importação/Exportação
 */
int btn_abrir_trade_cb(void) {
    trade_view_mostrar();
    return IUP_DEFAULT;
}

/*
 * Fecha o sistema
 */
int btn_sair_cb(void) {
    return IUP_CLOSE;
}

/* ========================================================================
 * FUNÇÃO PRINCIPAL
 * ======================================================================== */

int main(int argc, char **argv) {
    /* Inicializar biblioteca IUP (interface gráfica) */
    IupOpen(&argc, &argv);
    IupControlsOpen();
    IupSetGlobal("UTF8MODE", "YES");
    IupSetGlobal("UTF8MODE_FILE", "YES");

    /* Inicializar sistema de persistência (banco de dados) */
    pers_inicializar(TIPO_PERSISTENCIA);

    Ihandle *menu = IupMenu(
        IupSubmenu("Cadastros", IupMenu(
            IupItem("Produtora", "btn_abrir_produtora_cb"),
            IupItem("Clientes", "btn_abrir_clientes_cb"),
            IupItem("Equipe Interna", "btn_abrir_equipe_cb"), 
            IupItem("Recursos/Equipamentos", "btn_abrir_recursos_cb"),
            IupItem("Fornecedores/Parceiros", "btn_abrir_fornecedores_cb"),
            IupItem("Operadores do Sistema", "btn_abrir_operadores_cb"),
            IupSeparator(),
            IupItem("Sair", "btn_sair_cb")
            ,NULL
        )),
        IupSubmenu("Orçamentos e Eventos", IupMenu(
            IupItem("Abrir módulo", "btn_abrir_eventos_cb"),
            NULL
        )),
        IupSubmenu("Transações", IupMenu(
            IupItem("Abrir módulo", "btn_abrir_transacoes_cb"),
            NULL
        )),
        IupSubmenu("Relatórios", IupMenu(
            IupItem("Feedback e Relatórios", "btn_abrir_feedback_cb"),
            NULL
        )),
        IupSubmenu("Dados", IupMenu(
            IupItem("Importar/Exportar XML", "btn_abrir_trade_cb"),
            NULL
        )),
        NULL
    );

    Ihandle *lbl_title = IupLabel("Bem-vindo ao SGE \u2014 Sistema de Gest\u00e3o de Eventos");
    IupSetAttribute(lbl_title, "ALIGNMENT", "ACENTER:ACENTER");
    IupSetAttribute(lbl_title, "FONTSIZE", "18");
    Ihandle *lbl_hint = IupLabel("Use o menu superior para acessar Cadastros e Eventos.");
    IupSetAttribute(lbl_hint, "ALIGNMENT", "ACENTER:ACENTER");
    IupSetAttribute(lbl_hint, "FONTSIZE", "12");
    Ihandle *vcenter = IupVbox(lbl_title, lbl_hint, NULL);
    IupSetAttribute(vcenter, "ALIGNMENT", "ACENTER");
    IupSetAttribute(vcenter, "GAP", "8");
    Ihandle *hcenter = IupHbox(IupFill(), vcenter, IupFill(), NULL);
    Ihandle *content = IupVbox(IupFill(), hcenter, IupFill(), NULL);
    IupSetAttribute(content, "GAP", "6");

    Ihandle *dlg_main = IupDialog(content);
    IupSetAttribute(dlg_main, "TITLE", "SGE (Sistema de Gestão de Eventos)");
    IupSetAttribute(dlg_main, "SIZE", "720x480");
    
    IupSetAttributeHandle(dlg_main, "MENU", menu);
    
    IupSetFunction("btn_abrir_clientes_cb", (Icallback)btn_abrir_clientes_cb);
    IupSetFunction("btn_abrir_equipe_cb", (Icallback)btn_abrir_equipe_cb);
    IupSetFunction("btn_abrir_recursos_cb", (Icallback)btn_abrir_recursos_cb);
    IupSetFunction("btn_abrir_fornecedores_cb", (Icallback)btn_abrir_fornecedores_cb);
    IupSetFunction("btn_abrir_operadores_cb", (Icallback)btn_abrir_operadores_cb);
    IupSetFunction("btn_abrir_produtora_cb", (Icallback)btn_abrir_produtora_cb);
    IupSetFunction("btn_abrir_eventos_cb", (Icallback)btn_abrir_eventos_cb);
    IupSetFunction("btn_abrir_transacoes_cb", (Icallback)btn_abrir_transacoes_cb);
    IupSetFunction("btn_abrir_feedback_cb", (Icallback)btn_abrir_feedback_cb);
    IupSetFunction("btn_abrir_trade_cb", (Icallback)btn_abrir_trade_cb);
    IupSetFunction("btn_sair_cb", (Icallback)btn_sair_cb);

    IupShowXY(dlg_main, IUP_CENTER, IUP_CENTER);
    IupMainLoop();
    IupClose();
    return 0;
}