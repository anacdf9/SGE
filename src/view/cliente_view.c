#include <iup.h>
#include <iupcontrols.h>
#include "../controller/cliente_controller.h" // Inclui os callbacks (_cb)
#include "cliente_view.h" // Inclui o próprio .h (boa prática)
#include "ui_common.h"
#include "validation.h"
#include <stdlib.h>
#include <stdio.h>

// Guarda a janela do cliente pra não criar várias
static Ihandle* dlg_cliente = NULL;
static Ihandle* mat_cliente = NULL;

static void cliente_view_recarregar_matriz(Ihandle *mat) {
    Cliente buffer[256];
    int n = cliente_listar(buffer, 256);
    char numlin[16];
    sprintf(numlin, "%d", n > 0 ? n : 1);
    IupSetAttribute(mat, "NUMLIN", numlin);
    /* cabeçalhos */
    IupSetAttribute(mat, "NUMCOL", "7");
    IupSetAttribute(mat, "0:1", "ID");
    IupSetAttribute(mat, "0:2", "Nome");
    IupSetAttribute(mat, "0:3", "Endereço");
    IupSetAttribute(mat, "0:4", "CPF/CNPJ");
    IupSetAttribute(mat, "0:5", "Telefone");
    IupSetAttribute(mat, "0:6", "E-mail");
    IupSetAttribute(mat, "0:7", "Contato");
    for (int i = 1; i <= n; i++) {
        char buf[64];
        sprintf(buf, "%d", buffer[i-1].id);
        IupSetAttributeId2(mat, "", i, 1, buf);
        IupSetAttributeId2(mat, "", i, 2, buffer[i-1].nome);
        IupSetAttributeId2(mat, "", i, 3, buffer[i-1].endereco);
        IupSetAttributeId2(mat, "", i, 4, buffer[i-1].cpf_cnpj);
        IupSetAttributeId2(mat, "", i, 5, buffer[i-1].telefone);
        IupSetAttributeId2(mat, "", i, 6, buffer[i-1].email);
        IupSetAttributeId2(mat, "", i, 7, buffer[i-1].contato);
    }
}

static int cliente_view_click_cb(Ihandle *mat, int lin, int col, char *status) {
    if (lin <= 0) return IUP_DEFAULT;
    Ihandle *txtId = (Ihandle*)IupGetAttribute(mat, "txtId");
    Ihandle *txtNome = (Ihandle*)IupGetAttribute(mat, "txtNome");
    Ihandle *txtEndereco = (Ihandle*)IupGetAttribute(mat, "txtEndereco");
    Ihandle *txtCpfCnpj = (Ihandle*)IupGetAttribute(mat, "txtCpfCnpj");
    Ihandle *txtTelefone = (Ihandle*)IupGetAttribute(mat, "txtTelefone");
    Ihandle *txtEmail = (Ihandle*)IupGetAttribute(mat, "txtEmail");
    Ihandle *txtContato = (Ihandle*)IupGetAttribute(mat, "txtContato");
    const char *v;
    v = IupGetAttributeId2(mat, "", lin, 1); IupSetStrAttribute(txtId, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 2); IupSetStrAttribute(txtNome, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 3); IupSetStrAttribute(txtEndereco, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 4); IupSetStrAttribute(txtCpfCnpj, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 5); IupSetStrAttribute(txtTelefone, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 6); IupSetStrAttribute(txtEmail, "VALUE", v?v:"");
    v = IupGetAttributeId2(mat, "", lin, 7); IupSetStrAttribute(txtContato, "VALUE", v?v:"");
    return IUP_DEFAULT;
}

    static int cliente_salvar_wrap_cb(Ihandle *self) {
        Ihandle *txtCpfCnpj = (Ihandle*)IupGetAttribute(self, "txtCpfCnpj");
        Ihandle *txtTelefone = (Ihandle*)IupGetAttribute(self, "txtTelefone");
        const char* sdoc = IupGetAttribute(txtCpfCnpj, "VALUE");
        const char* stel = IupGetAttribute(txtTelefone, "VALUE");
        if (!ui_valid_cpf_cnpj(sdoc)) { IupMessage("Dados inválidos", "CPF/CNPJ deve conter 11 ou 14 dígitos."); return IUP_DEFAULT; }
        if (!ui_valid_phone(stel)) { IupMessage("Dados inválidos", "Telefone deve conter 10 ou 11 dígitos."); return IUP_DEFAULT; }
        cliente_salvar_cb(self);
        if (mat_cliente) cliente_view_recarregar_matriz(mat_cliente);
        return IUP_DEFAULT;
    }

    static int cliente_excluir_wrap_cb(Ihandle *self) {
        cliente_excluir_cb(self);
        if (mat_cliente) cliente_view_recarregar_matriz(mat_cliente);
        return IUP_DEFAULT;
    }

static int cliente_atualizar_cb(Ihandle *self) {
    Ihandle *m = (Ihandle*)IupGetAttribute(self, "MATRIX");
    cliente_view_recarregar_matriz(m);
    return IUP_DEFAULT;
}

/*
 * Função que cria a tela de cadastro de cliente
 * (Essa função é interna, só a 'mostrar' chama)
 */
Ihandle* cliente_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    Ihandle *txtNome = IupText(NULL);
    Ihandle *txtEndereco = IupText(NULL);
    Ihandle *txtCpfCnpj = IupText(NULL);
    Ihandle *txtTelefone = IupText(NULL);
    Ihandle *txtEmail = IupText(NULL);
    Ihandle *txtContato = IupText(NULL);

    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    mat_cliente = IupMatrix(NULL);
    IupSetAttribute(mat_cliente, "NUMCOL", "7");
    IupSetAttribute(mat_cliente, "NUMLIN", "1");
    IupSetAttribute(mat_cliente, "EXPAND", "YES");
    IupSetAttribute(mat_cliente, "WIDTH1", "40");
    IupSetAttribute(mat_cliente, "WIDTH2", "150");
    IupSetAttribute(mat_cliente, "WIDTH3", "180");
    IupSetAttribute(mat_cliente, "WIDTH4", "100");
    IupSetAttribute(mat_cliente, "WIDTH5", "100");
    IupSetAttribute(mat_cliente, "WIDTH6", "160");
    IupSetAttribute(mat_cliente, "WIDTH7", "120");

    // Monta o formulário
    /* Larguras por tipo de dado */
    ui_set_width_px(txtId, UI_W_ID);
    ui_set_width_px(txtCpfCnpj, UI_W_MED);
    ui_set_width_px(txtNome, UI_W_XLONG);
    ui_set_width_px(txtContato, UI_W_MED);
    ui_set_width_px(txtEndereco, UI_W_XLONG);
    ui_set_width_px(txtTelefone, UI_W_MED);
    ui_set_width_px(txtEmail, UI_W_XLONG);
    IupSetAttribute(txtCpfCnpj, "TIP", "Somente dígitos: CPF=11, CNPJ=14");
    IupSetAttribute(txtTelefone, "TIP", "Somente dígitos: Tel=10-11");

    Ihandle *rows = IupVbox(
        ui_pair("Código:", txtId),
        ui_pair("CPF/CNPJ:", txtCpfCnpj),
        ui_pair("Nome/Razão Social:", txtNome),
        ui_pair("Contato:", txtContato),
        ui_pair("Endereço:", txtEndereco),
        ui_pair("Telefone:", txtTelefone),
        ui_pair("E-mail:", txtEmail),
        NULL
    );
    ui_style_form(rows);

    Ihandle *form_vbox = IupVbox(
        IupLabel("Cadastro de Cliente"),
        rows,
        ui_buttons_center(btnSalvar, btnExcluir, btnAtualizar),
        IupSetAttributes(IupFrame(mat_cliente), "TITLE=Lista de Clientes"),
        NULL
    );
    ui_style_form(form_vbox);
    
    // Associa os handles dos campos de texto aos botões
    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtNome", (char*)txtNome);
    IupSetAttribute(btnSalvar, "txtEndereco", (char*)txtEndereco);
    IupSetAttribute(btnSalvar, "txtCpfCnpj", (char*)txtCpfCnpj);
    IupSetAttribute(btnSalvar, "txtTelefone", (char*)txtTelefone);
    IupSetAttribute(btnSalvar, "txtEmail", (char*)txtEmail);
    IupSetAttribute(btnSalvar, "txtContato", (char*)txtContato);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_cliente);
    /* tamanhos melhores */
    /* validação simples antes de salvar */

    /* vincula campos à matriz para carregar ao clicar */
    IupSetAttribute(mat_cliente, "txtId", (char*)txtId);
    IupSetAttribute(mat_cliente, "txtNome", (char*)txtNome);
    IupSetAttribute(mat_cliente, "txtEndereco", (char*)txtEndereco);
    IupSetAttribute(mat_cliente, "txtCpfCnpj", (char*)txtCpfCnpj);
    IupSetAttribute(mat_cliente, "txtTelefone", (char*)txtTelefone);
    IupSetAttribute(mat_cliente, "txtEmail", (char*)txtEmail);
    IupSetAttribute(mat_cliente, "txtContato", (char*)txtContato);

    // Define os callbacks (usando os nomes com _cb)
    IupSetCallback(btnSalvar, "ACTION", (Icallback)cliente_salvar_wrap_cb);
    IupSetCallback(btnExcluir, "ACTION", (Icallback)cliente_excluir_wrap_cb);
    IupSetCallback(btnAtualizar, "ACTION", (Icallback)cliente_atualizar_cb);
    IupSetCallback(mat_cliente, "CLICK_CB", (Icallback)cliente_view_click_cb);

    cliente_view_recarregar_matriz(mat_cliente);

    return form_vbox; // Retorna o Vbox (o formulário)
}


/*
 * FUNÇÃO QUE ESTAVA FALTANDO
 * É chamada pelo main.c para mostrar a tela
 */
void cliente_view_mostrar(void) {
    // Se a dialog ainda não foi criada, cria
    if (dlg_cliente == NULL) {
        
        // 1. Cria o formulário (o Vbox)
        Ihandle* form_cliente = cliente_view_create();
        
        // 2. Coloca o formulário dentro de uma Dialog (Janela)
        dlg_cliente = IupDialog(form_cliente);
        IupSetAttribute(dlg_cliente, "TITLE", "Cadastro de Clientes");
        IupSetAttribute(dlg_cliente, "SIZE", "700x420");
    }

    // Mostra a dialog (janela)
    IupShowXY(dlg_cliente, IUP_CENTER, IUP_CENTER);
}