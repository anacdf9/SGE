/*
===============================================================================
   RECURSO VIEW - Interface Gráfica
   
   Tela de cadastro de recursos/equipamentos com:
   - Matriz listando recursos e preços
   - Formulário para criar/editar recursos
   - Campos: descrição, categoria, quantidade, custo, locação
   - Botões: Novo, Salvar, Excluir, Atualizar
   
   IMPORTANTE: O preço de locação é calculado automaticamente nas compras!
===============================================================================
*/

#include <iup.h>
#include <iupcontrols.h>
#include "../controller/recurso_controller.h"
#include "ui_common.h"
#include <stdlib.h>
#include <stdio.h>


/* ========================================
   VARIÁVEIS GLOBAIS DA TELA
   ======================================== */

static Ihandle *dlg_recurso = NULL;
static Ihandle *mat_recurso = NULL;


/* ========================================
   FUNÇÕES AUXILIARES
   ======================================== */

// Recarrega a matriz com todos os recursos cadastrados
static void recurso_recarregar_matriz(Ihandle *mat) {
    Recurso buffer[256];
    int n = recurso_listar(buffer, 256);
    
    char numlin[16];
    sprintf(numlin, "%d", n > 0 ? n : 1);
    
    // Configurar colunas
    IupSetAttribute(mat, "NUMCOL", "6");
    IupSetAttribute(mat, "NUMLIN", numlin);
    IupSetAttribute(mat, "0:1", "ID");
    IupSetAttribute(mat, "0:2", "Descrição");
    IupSetAttribute(mat, "0:3", "Categoria");
    IupSetAttribute(mat, "0:4", "Qtd");
    IupSetAttribute(mat, "0:5", "Custo");
    IupSetAttribute(mat, "0:6", "Locação");
    
    // Preencher linhas
    for (int i = 1; i <= n; i++) {
        char idbuf[32], qbuf[32], cbuf[32], lbuf[32];
        
        sprintf(idbuf, "%d", buffer[i-1].id);
        sprintf(qbuf, "%d", buffer[i-1].quantidade);
        sprintf(cbuf, "%.2f", buffer[i-1].preco_custo);
        sprintf(lbuf, "%.2f", buffer[i-1].valor_locacao);
        
        IupSetStrAttributeId2(mat, "", i, 1, idbuf);
        IupSetStrAttributeId2(mat, "", i, 2, buffer[i-1].descricao);
        IupSetStrAttributeId2(mat, "", i, 3, buffer[i-1].categoria);
        IupSetStrAttributeId2(mat, "", i, 4, qbuf);
        IupSetStrAttributeId2(mat, "", i, 5, cbuf);
        IupSetStrAttributeId2(mat, "", i, 6, lbuf);
    }
}

// Callback quando usuário clica em uma linha da matriz
static int recurso_click_cb(Ihandle *mat, int lin, int col, char *status) {
    if (lin <= 0) return IUP_DEFAULT;
    
    // Obter ponteiros dos campos
    Ihandle *tId = (Ihandle*)IupGetAttribute(mat, "txtId");
    Ihandle *tDesc = (Ihandle*)IupGetAttribute(mat, "txtDesc");
    Ihandle *tCat = (Ihandle*)IupGetAttribute(mat, "txtCat");
    Ihandle *tQtd = (Ihandle*)IupGetAttribute(mat, "txtQtd");
    Ihandle *tCusto = (Ihandle*)IupGetAttribute(mat, "txtCusto");
    Ihandle *tLoc = (Ihandle*)IupGetAttribute(mat, "txtLoc");
    
    // Preencher formulário com dados da linha selecionada
    const char *v;
    v = IupGetAttributeId2(mat, "", lin, 1);
    IupSetStrAttribute(tId, "VALUE", v ? v : "");
    v = IupGetAttributeId2(mat, "", lin, 2);
    IupSetStrAttribute(tDesc, "VALUE", v ? v : "");
    v = IupGetAttributeId2(mat, "", lin, 3);
    IupSetStrAttribute(tCat, "VALUE", v ? v : "");
    v = IupGetAttributeId2(mat, "", lin, 4);
    IupSetStrAttribute(tQtd, "VALUE", v ? v : "");
    v = IupGetAttributeId2(mat, "", lin, 5);
    IupSetStrAttribute(tCusto, "VALUE", v ? v : "");
    v = IupGetAttributeId2(mat, "", lin, 6);
    IupSetStrAttribute(tLoc, "VALUE", v ? v : "");
    
    return IUP_DEFAULT;
}


/* ========================================
   CALLBACKS DOS BOTÕES
   ======================================== */

// Callback do botão Atualizar
static int recurso_atualizar_cb(Ihandle *self) {
    Ihandle *m = (Ihandle*)IupGetAttribute(self, "MATRIX");
    recurso_recarregar_matriz(m);
    return IUP_DEFAULT;
}

// Callback do botão Salvar
static int recurso_salvar_wrap_cb(Ihandle *self) {
    recurso_salvar_cb(self);
    if (mat_recurso) recurso_recarregar_matriz(mat_recurso);
    return IUP_DEFAULT;
}

// Callback do botão Excluir
static int recurso_excluir_wrap_cb(Ihandle *self) {
    recurso_excluir_cb(self);
    if (mat_recurso) recurso_recarregar_matriz(mat_recurso);
    return IUP_DEFAULT;
}

// Callback do botão Novo (limpa formulário)
static int recurso_novo_cb(Ihandle *self) {
    Ihandle *txtId=(Ihandle*)IupGetAttribute(self,"txtId");
    Ihandle *txtDesc=(Ihandle*)IupGetAttribute(self,"txtDesc");
    Ihandle *txtCat=(Ihandle*)IupGetAttribute(self,"txtCat");
    Ihandle *txtQtd=(Ihandle*)IupGetAttribute(self,"txtQtd");
    Ihandle *txtCusto=(Ihandle*)IupGetAttribute(self,"txtCusto");
    Ihandle *txtLoc=(Ihandle*)IupGetAttribute(self,"txtLoc");
    IupSetAttribute(txtId,"VALUE","");
    IupSetAttribute(txtDesc,"VALUE","");
    IupSetAttribute(txtCat,"VALUE","");
    IupSetAttribute(txtQtd,"VALUE","");
    IupSetAttribute(txtCusto,"VALUE","");
    IupSetAttribute(txtLoc,"VALUE","");
    IupSetFocus(txtDesc);
    return IUP_DEFAULT;
}

static Ihandle* recurso_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    IupSetAttribute(txtId, "READONLY", "YES");
    IupSetAttribute(txtId, "TIP", "Gerado automaticamente");
    Ihandle *txtDesc = IupText(NULL);
    Ihandle *txtCat = IupText(NULL);
    Ihandle *txtQtd = IupText(NULL);
    Ihandle *txtCusto = IupText(NULL);
    Ihandle *txtLoc = IupText(NULL);

    Ihandle *btnNovo = IupButton("Novo", NULL);
    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    mat_recurso = IupMatrix(NULL);
    IupSetAttribute(mat_recurso,"NUMCOL","6"); IupSetAttribute(mat_recurso,"NUMLIN","1"); IupSetAttribute(mat_recurso,"EXPAND","YES");
    IupSetAttribute(mat_recurso,"WIDTH1","40"); IupSetAttribute(mat_recurso,"WIDTH2","160"); IupSetAttribute(mat_recurso,"WIDTH3","120");
    IupSetAttribute(mat_recurso,"WIDTH4","50"); IupSetAttribute(mat_recurso,"WIDTH5","80"); IupSetAttribute(mat_recurso,"WIDTH6","80");

    ui_set_width_px(txtId, UI_W_ID);
    ui_set_width_px(txtCat, UI_W_LONG);
    ui_set_width_px(txtDesc, UI_W_XLONG);
    ui_set_width_px(txtQtd, UI_W_SHORT);
    ui_set_width_px(txtCusto, UI_W_MED);
    ui_set_width_px(txtLoc, UI_W_MED);

    Ihandle *rows = IupVbox(
        ui_pair("Código:", txtId),
        ui_pair("Categoria:", txtCat),
        ui_pair("Descrição:", txtDesc),
        ui_pair("Quantidade:", txtQtd),
        ui_pair("Preço de Custo:", txtCusto),
        ui_pair("Valor da Locação:", txtLoc),
        NULL
    );
    ui_style_form(rows);

    Ihandle *btn_row = IupHbox(IupFill(), btnNovo, btnSalvar, btnExcluir, btnAtualizar, IupFill(), NULL);
    Ihandle *form = IupVbox(
        IupLabel("Cadastro de Recursos/Equipamentos"),
        rows,
        btn_row,
        IupSetAttributes(IupFrame(mat_recurso), "TITLE=Lista de Recursos"),
        NULL
    );
    ui_style_form(form);

    IupSetAttribute(btnNovo, "txtId", (char*)txtId);
    IupSetAttribute(btnNovo, "txtDesc", (char*)txtDesc);
    IupSetAttribute(btnNovo, "txtCat", (char*)txtCat);
    IupSetAttribute(btnNovo, "txtQtd", (char*)txtQtd);
    IupSetAttribute(btnNovo, "txtCusto", (char*)txtCusto);
    IupSetAttribute(btnNovo, "txtLoc", (char*)txtLoc);

    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtDesc", (char*)txtDesc);
    IupSetAttribute(btnSalvar, "txtCat", (char*)txtCat);
    IupSetAttribute(btnSalvar, "txtQtd", (char*)txtQtd);
    IupSetAttribute(btnSalvar, "txtCusto", (char*)txtCusto);
    IupSetAttribute(btnSalvar, "txtLoc", (char*)txtLoc);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_recurso);
    /* tamanhos padronizados aplicados por ui_pair */

    IupSetAttribute(mat_recurso,"txtId",(char*)txtId); IupSetAttribute(mat_recurso,"txtDesc",(char*)txtDesc);
    IupSetAttribute(mat_recurso,"txtCat",(char*)txtCat); IupSetAttribute(mat_recurso,"txtQtd",(char*)txtQtd);
    IupSetAttribute(mat_recurso,"txtCusto",(char*)txtCusto); IupSetAttribute(mat_recurso,"txtLoc",(char*)txtLoc);

    IupSetCallback(btnNovo, "ACTION", (Icallback)recurso_novo_cb);
    IupSetCallback(btnSalvar, "ACTION", (Icallback)recurso_salvar_wrap_cb);
    IupSetCallback(btnExcluir, "ACTION", (Icallback)recurso_excluir_wrap_cb);
    IupSetCallback(btnAtualizar, "ACTION", (Icallback)recurso_atualizar_cb);
    IupSetCallback(mat_recurso, "CLICK_CB", (Icallback)recurso_click_cb);

    recurso_recarregar_matriz(mat_recurso);

    return form;
}

void recurso_view_mostrar(void) {
    if (!dlg_recurso) {
        dlg_recurso = IupDialog(recurso_view_create());
        IupSetAttribute(dlg_recurso, "TITLE", "Cadastro de Recursos");
        IupSetAttribute(dlg_recurso, "SIZE", "680x520");
        IupSetAttribute(dlg_recurso, "RESIZE", "YES");
        IupSetAttribute(dlg_recurso, "MAXBOX", "YES");
    }
    IupShowXY(dlg_recurso, IUP_CENTER, IUP_CENTER);
}
