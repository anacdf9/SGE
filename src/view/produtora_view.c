#include <iup.h>
#include <iupcontrols.h>
#include "../controller/produtora_controller.h"
#include "ui_common.h"
#include "validation.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static Ihandle *dlg_produtora = NULL;
static Ihandle *mat_prod = NULL;

/* declaração prévia para uso inicial */
static void produtora_recarregar_matriz(Ihandle *mat);

static int produtora_salvar_wrap_cb(Ihandle *self) {
    Ihandle *tCNPJ = (Ihandle*)IupGetAttribute(self, "tCNPJ");
    Ihandle *tTel = (Ihandle*)IupGetAttribute(self, "tTel");
    Ihandle *tTelResp = (Ihandle*)IupGetAttribute(self, "tTelResp");
    const char* cnpj = IupGetAttribute(tCNPJ, "VALUE");
    const char* tel = IupGetAttribute(tTel, "VALUE");
    const char* telr = IupGetAttribute(tTelResp, "VALUE");
    if (!ui_valid_cpf_cnpj(cnpj) || (cnpj && (int)strlen(cnpj) < 14)) { IupMessage("Dados inválidos", "CNPJ deve conter 14 dígitos."); return IUP_DEFAULT; }
    if (tel && tel[0] && !ui_valid_phone(tel)) { IupMessage("Dados inválidos", "Telefone deve conter 10 ou 11 dígitos."); return IUP_DEFAULT; }
    if (telr && telr[0] && !ui_valid_phone(telr)) { IupMessage("Dados inválidos", "Tel. Responsável deve conter 10 ou 11 dígitos."); return IUP_DEFAULT; }
    int rc = produtora_salvar_cb(self);
    if (mat_prod) produtora_recarregar_matriz(mat_prod);
    return rc;
}

static void produtora_recarregar_matriz(Ihandle *mat) {
    Produtora p; int has = produtora_obter(&p);
    IupSetAttribute(mat, "NUMCOL", "10");
    /* limpar linhas para evitar lixo residual */
    IupSetAttribute(mat, "NUMLIN", "0");
    IupSetAttribute(mat, "NUMLIN", "1");
    IupSetAttribute(mat, "0:1", "Nome Fantasia");
    IupSetAttribute(mat, "0:2", "Razão Social");
    IupSetAttribute(mat, "0:3", "Inscr. Est.");
    IupSetAttribute(mat, "0:4", "CNPJ");
    IupSetAttribute(mat, "0:5", "Endereço");
    IupSetAttribute(mat, "0:6", "Telefone");
    IupSetAttribute(mat, "0:7", "E-mail");
    IupSetAttribute(mat, "0:8", "Responsável");
    IupSetAttribute(mat, "0:9", "Tel. Resp.");
    IupSetAttribute(mat, "0:10", "Margem %");
    /* inicializar linha com strings vazias para evitar lixo quando não há dados */
    for (int c = 1; c <= 10; c++) {
        IupSetStrAttributeId2(mat, "", 1, c, "");
    }
    if (has) {
        char margem[32]; sprintf(margem, "%.2f", p.margem_lucro_padrao);
        IupSetStrAttributeId2(mat, "", 1, 1, p.nome_fantasia);
        IupSetStrAttributeId2(mat, "", 1, 2, p.razao_social);
        IupSetStrAttributeId2(mat, "", 1, 3, p.inscricao_estadual);
        IupSetStrAttributeId2(mat, "", 1, 4, p.cnpj);
        IupSetStrAttributeId2(mat, "", 1, 5, p.endereco);
        IupSetStrAttributeId2(mat, "", 1, 6, p.telefone);
        IupSetStrAttributeId2(mat, "", 1, 7, p.email);
        IupSetStrAttributeId2(mat, "", 1, 8, p.responsavel);
        IupSetStrAttributeId2(mat, "", 1, 9, p.telefone_responsavel);
        IupSetStrAttributeId2(mat, "", 1, 10, margem);
    }
}

static int produtora_click_cb(Ihandle *mat, int lin, int col, char *status) {
    if (lin <= 0) return IUP_DEFAULT;
    Ihandle *tNF = (Ihandle*)IupGetAttribute(mat, "tNF");
    Ihandle *tRS = (Ihandle*)IupGetAttribute(mat, "tRS");
    Ihandle *tIE = (Ihandle*)IupGetAttribute(mat, "tIE");
    Ihandle *tCNPJ = (Ihandle*)IupGetAttribute(mat, "tCNPJ");
    Ihandle *tEnd = (Ihandle*)IupGetAttribute(mat, "tEnd");
    Ihandle *tTel = (Ihandle*)IupGetAttribute(mat, "tTel");
    Ihandle *tEmail = (Ihandle*)IupGetAttribute(mat, "tEmail");
    Ihandle *tResp = (Ihandle*)IupGetAttribute(mat, "tResp");
    Ihandle *tTelResp = (Ihandle*)IupGetAttribute(mat, "tTelResp");
    Ihandle *tMargem = (Ihandle*)IupGetAttribute(mat, "tMargem");
    const char *v;
    v = IupGetAttributeId2(mat, "", lin, 1); IupSetStrAttribute(tNF, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 2); IupSetStrAttribute(tRS, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 3); IupSetStrAttribute(tIE, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 4); IupSetStrAttribute(tCNPJ, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 5); IupSetStrAttribute(tEnd, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 6); IupSetStrAttribute(tTel, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 7); IupSetStrAttribute(tEmail, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 8); IupSetStrAttribute(tResp, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 9); IupSetStrAttribute(tTelResp, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 10); IupSetStrAttribute(tMargem, "VALUE", v?v:"");
    return IUP_DEFAULT;
}

static int produtora_atualizar_cb(Ihandle *self) {
    Ihandle *m = (Ihandle*)IupGetAttribute(self, "MATRIX");
    produtora_recarregar_matriz(m);
    return IUP_DEFAULT;
}

static int produtora_excluir_wrap_cb(Ihandle *self) {
    produtora_excluir_cb(self);
    if (mat_prod) produtora_recarregar_matriz(mat_prod);
    return IUP_DEFAULT;
}

static Ihandle* produtora_view_create(void) {
    Ihandle *tNF = IupText(NULL);
    Ihandle *tRS = IupText(NULL);
    Ihandle *tIE = IupText(NULL);
    Ihandle *tCNPJ = IupText(NULL);
    Ihandle *tEnd = IupText(NULL);
    Ihandle *tTel = IupText(NULL);
    Ihandle *tEmail = IupText(NULL);
    Ihandle *tResp = IupText(NULL);
    Ihandle *tTelResp = IupText(NULL);
    Ihandle *tMargem = IupText(NULL);

    /* garantir valores iniciais limpos */
    IupSetAttribute(tNF, "VALUE", "");
    IupSetAttribute(tRS, "VALUE", "");
    IupSetAttribute(tIE, "VALUE", "");
    IupSetAttribute(tCNPJ, "VALUE", "");
    IupSetAttribute(tEnd, "VALUE", "");
    IupSetAttribute(tTel, "VALUE", "");
    IupSetAttribute(tEmail, "VALUE", "");
    IupSetAttribute(tResp, "VALUE", "");
    IupSetAttribute(tTelResp, "VALUE", "");
    IupSetAttribute(tMargem, "VALUE", "");

    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnCarregar = IupButton("Carregar", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    ui_set_width_px(tNF, UI_W_XLONG);
    ui_set_width_px(tRS, UI_W_XLONG);
    ui_set_width_px(tIE, UI_W_MED);
    ui_set_width_px(tCNPJ, UI_W_MED);
    ui_set_width_px(tEnd, UI_W_XLONG);
    ui_set_width_px(tTel, UI_W_MED);
    ui_set_width_px(tEmail, UI_W_XLONG);
    ui_set_width_px(tResp, UI_W_LONG);
    ui_set_width_px(tTelResp, UI_W_MED);
    ui_set_width_px(tMargem, UI_W_SHORT);

    IupSetAttribute(tCNPJ, "TIP", "Somente dígitos: CNPJ=14");
    IupSetAttribute(tTel, "TIP", "Somente dígitos: Tel=10-11");
    IupSetAttribute(tTelResp, "TIP", "Somente dígitos: Tel=10-11");

    Ihandle *rows = IupVbox(
        ui_pair("Nome Fantasia:", tNF),
        ui_pair("Razão Social:", tRS),
        ui_pair("Inscrição Estadual:", tIE),
        ui_pair("CNPJ:", tCNPJ),
        ui_pair("Endereço:", tEnd),
        ui_pair("Telefone:", tTel),
        ui_pair("E-mail:", tEmail),
        ui_pair("Responsável:", tResp),
        ui_pair("Tel. Responsável:", tTelResp),
        ui_pair("Margem Lucro (%):", tMargem),
        NULL
    );
    ui_style_form(rows);

    mat_prod = IupMatrix(NULL);
    IupSetAttribute(mat_prod, "NUMCOL", "10");
    IupSetAttribute(mat_prod, "NUMLIN", "1");
    IupSetAttribute(mat_prod, "EXPAND", "YES");
    IupSetAttribute(mat_prod, "WIDTH1", "180");
    IupSetAttribute(mat_prod, "WIDTH2", "180");
    IupSetAttribute(mat_prod, "WIDTH3", "100");
    IupSetAttribute(mat_prod, "WIDTH4", "120");
    IupSetAttribute(mat_prod, "WIDTH5", "220");
    IupSetAttribute(mat_prod, "WIDTH6", "120");
    IupSetAttribute(mat_prod, "WIDTH7", "200");
    IupSetAttribute(mat_prod, "WIDTH8", "160");
    IupSetAttribute(mat_prod, "WIDTH9", "120");
    IupSetAttribute(mat_prod, "WIDTH10", "80");

    Ihandle *buttons_row1 = ui_buttons_center(btnSalvar, btnExcluir, btnCarregar);
    Ihandle *buttons_row2 = IupHbox(IupFill(), btnAtualizar, IupFill(), NULL);
    Ihandle *form = IupVbox(
        IupLabel("Dados da Produtora (Cadastro Único)"),
        rows,
        buttons_row1,
        buttons_row2,
        IupSetAttributes(IupFrame(mat_prod), "TITLE=Consulta"),
        NULL
    );
    ui_style_form(form);

    IupSetAttribute(btnSalvar, "tNF", (char*)tNF);
    IupSetAttribute(btnSalvar, "tRS", (char*)tRS);
    IupSetAttribute(btnSalvar, "tIE", (char*)tIE);
    IupSetAttribute(btnSalvar, "tCNPJ", (char*)tCNPJ);
    IupSetAttribute(btnSalvar, "tEnd", (char*)tEnd);
    IupSetAttribute(btnSalvar, "tTel", (char*)tTel);
    IupSetAttribute(btnSalvar, "tEmail", (char*)tEmail);
    IupSetAttribute(btnSalvar, "tResp", (char*)tResp);
    IupSetAttribute(btnSalvar, "tTelResp", (char*)tTelResp);
    IupSetAttribute(btnSalvar, "tMargem", (char*)tMargem);

    IupSetAttribute(btnCarregar, "tNF", (char*)tNF);
    IupSetAttribute(btnCarregar, "tRS", (char*)tRS);
    IupSetAttribute(btnCarregar, "tIE", (char*)tIE);
    IupSetAttribute(btnCarregar, "tCNPJ", (char*)tCNPJ);
    IupSetAttribute(btnCarregar, "tEnd", (char*)tEnd);
    IupSetAttribute(btnCarregar, "tTel", (char*)tTel);
    IupSetAttribute(btnCarregar, "tEmail", (char*)tEmail);
    IupSetAttribute(btnCarregar, "tResp", (char*)tResp);
    IupSetAttribute(btnCarregar, "tTelResp", (char*)tTelResp);
    IupSetAttribute(btnCarregar, "tMargem", (char*)tMargem);

    IupSetCallback(btnSalvar, "ACTION", (Icallback)produtora_salvar_wrap_cb);
    IupSetAttribute(btnExcluir, "tNF", (char*)tNF);
    IupSetAttribute(btnExcluir, "tRS", (char*)tRS);
    IupSetAttribute(btnExcluir, "tIE", (char*)tIE);
    IupSetAttribute(btnExcluir, "tCNPJ", (char*)tCNPJ);
    IupSetAttribute(btnExcluir, "tEnd", (char*)tEnd);
    IupSetAttribute(btnExcluir, "tTel", (char*)tTel);
    IupSetAttribute(btnExcluir, "tEmail", (char*)tEmail);
    IupSetAttribute(btnExcluir, "tResp", (char*)tResp);
    IupSetAttribute(btnExcluir, "tTelResp", (char*)tTelResp);
    IupSetAttribute(btnExcluir, "tMargem", (char*)tMargem);
    IupSetCallback(btnExcluir, "ACTION", (Icallback)produtora_excluir_wrap_cb);
    IupSetCallback(btnCarregar, "ACTION", (Icallback)produtora_carregar_cb);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_prod);
    IupSetCallback(btnAtualizar, "ACTION", (Icallback)produtora_atualizar_cb);

    /* passar campos para a matriz para click popular os inputs */
    IupSetAttribute(mat_prod, "tNF", (char*)tNF);
    IupSetAttribute(mat_prod, "tRS", (char*)tRS);
    IupSetAttribute(mat_prod, "tIE", (char*)tIE);
    IupSetAttribute(mat_prod, "tCNPJ", (char*)tCNPJ);
    IupSetAttribute(mat_prod, "tEnd", (char*)tEnd);
    IupSetAttribute(mat_prod, "tTel", (char*)tTel);
    IupSetAttribute(mat_prod, "tEmail", (char*)tEmail);
    IupSetAttribute(mat_prod, "tResp", (char*)tResp);
    IupSetAttribute(mat_prod, "tTelResp", (char*)tTelResp);
    IupSetAttribute(mat_prod, "tMargem", (char*)tMargem);
    IupSetCallback(mat_prod, "CLICK_CB", (Icallback)produtora_click_cb);

    produtora_recarregar_matriz(mat_prod);
    /* carregar automaticamente os dados salvos nos campos, se existirem */
    produtora_carregar_cb(btnCarregar);
    return form;
}

void produtora_view_mostrar(void) {
    if (!dlg_produtora) {
        dlg_produtora = IupDialog(produtora_view_create());
        IupSetAttribute(dlg_produtora, "TITLE", "Produtora de Eventos");
        IupSetAttribute(dlg_produtora, "SIZE", "700x420");
    }
    IupShowXY(dlg_produtora, IUP_CENTER, IUP_CENTER);
}
