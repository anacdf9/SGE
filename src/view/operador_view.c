#include <iup.h>
#include <iupcontrols.h>
#include "../controller/operador_controller.h"
#include "ui_common.h"
#include <stdlib.h>
#include <stdio.h>

static Ihandle *dlg_operador = NULL;
static Ihandle *mat_oper = NULL;

static void operador_recarregar_matriz(Ihandle *mat){ Operador buf[256]; int n=operador_listar(buf,256); char k[16]; sprintf(k,"%d",n>0?n:1); IupSetAttribute(mat,"NUMCOL","4"); IupSetAttribute(mat,"NUMLIN",k); IupSetAttribute(mat,"0:1","ID"); IupSetAttribute(mat,"0:2","Nome"); IupSetAttribute(mat,"0:3","Usuário"); IupSetAttribute(mat,"0:4","Senha"); for(int i=1;i<=n;i++){ char idb[32]; sprintf(idb,"%d",buf[i-1].id); IupSetStrAttributeId2(mat,"",i,1,idb); IupSetStrAttributeId2(mat,"",i,2,buf[i-1].nome); IupSetStrAttributeId2(mat,"",i,3,buf[i-1].usuario); IupSetStrAttributeId2(mat,"",i,4,buf[i-1].senha);} }
static int operador_click_cb(Ihandle *mat,int lin,int col,char *status){ if(lin<=0) return IUP_DEFAULT; Ihandle *tId=(Ihandle*)IupGetAttribute(mat,"txtId"); Ihandle *tNome=(Ihandle*)IupGetAttribute(mat,"txtNome"); Ihandle *tUser=(Ihandle*)IupGetAttribute(mat,"txtUser"); Ihandle *tSenha=(Ihandle*)IupGetAttribute(mat,"txtSenha"); const char *v; v=IupGetAttributeId2(mat,"",lin,1); IupSetStrAttribute(tId,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,2); IupSetStrAttribute(tNome,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,3); IupSetStrAttribute(tUser,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,4); IupSetStrAttribute(tSenha,"VALUE",v?v:""); return IUP_DEFAULT; }
static int operador_atualizar_cb(Ihandle *self){ Ihandle *m=(Ihandle*)IupGetAttribute(self,"MATRIX"); operador_recarregar_matriz(m); return IUP_DEFAULT; }

static int operador_salvar_wrap_cb(Ihandle *self){
    operador_salvar_cb(self);
    if (mat_oper) operador_recarregar_matriz(mat_oper);
    return IUP_DEFAULT;
}

static int operador_excluir_wrap_cb(Ihandle *self){
    operador_excluir_cb(self);
    if (mat_oper) operador_recarregar_matriz(mat_oper);
    return IUP_DEFAULT;
}

static int operador_novo_cb(Ihandle *self){
    Ihandle *txtId=(Ihandle*)IupGetAttribute(self,"txtId");
    Ihandle *txtNome=(Ihandle*)IupGetAttribute(self,"txtNome");
    Ihandle *txtUser=(Ihandle*)IupGetAttribute(self,"txtUser");
    Ihandle *txtSenha=(Ihandle*)IupGetAttribute(self,"txtSenha");
    IupSetAttribute(txtId,"VALUE","");
    IupSetAttribute(txtNome,"VALUE","");
    IupSetAttribute(txtUser,"VALUE","");
    IupSetAttribute(txtSenha,"VALUE","");
    IupSetFocus(txtUser);
    return IUP_DEFAULT;
}

static Ihandle* operador_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    IupSetAttribute(txtId, "READONLY", "YES");
    IupSetAttribute(txtId, "TIP", "Gerado automaticamente");
    Ihandle *txtNome = IupText(NULL);
    Ihandle *txtUser = IupText(NULL);
    Ihandle *txtSenha = IupText(NULL);
    IupSetAttribute(txtSenha, "PASSWORD", "YES");

    Ihandle *btnNovo = IupButton("Novo", NULL);
    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    mat_oper = IupMatrix(NULL);
    IupSetAttribute(mat_oper,"NUMCOL","4"); IupSetAttribute(mat_oper,"NUMLIN","1"); IupSetAttribute(mat_oper,"EXPAND","YES");
    IupSetAttribute(mat_oper,"WIDTH1","40"); IupSetAttribute(mat_oper,"WIDTH2","160"); IupSetAttribute(mat_oper,"WIDTH3","140"); IupSetAttribute(mat_oper,"WIDTH4","120");

    ui_set_width_px(txtId, UI_W_ID);
    ui_set_width_px(txtUser, UI_W_MED);
    ui_set_width_px(txtNome, UI_W_LONG);
    ui_set_width_px(txtSenha, UI_W_MED);

    Ihandle *rows = IupVbox(
        ui_pair("Código:", txtId),
        ui_pair("Usuário:", txtUser),
        ui_pair("Nome:", txtNome),
        ui_pair("Senha:", txtSenha),
        NULL
    );
    ui_style_form(rows);

    Ihandle *btn_row = IupHbox(IupFill(), btnNovo, btnSalvar, btnExcluir, btnAtualizar, IupFill(), NULL);
    Ihandle *form = IupVbox(
        IupLabel("Cadastro de Operadores do Sistema"),
        rows,
        btn_row,
        IupSetAttributes(IupFrame(mat_oper), "TITLE=Lista de Operadores"),
        NULL
    );
    ui_style_form(form);

    IupSetAttribute(btnNovo, "txtId", (char*)txtId);
    IupSetAttribute(btnNovo, "txtNome", (char*)txtNome);
    IupSetAttribute(btnNovo, "txtUser", (char*)txtUser);
    IupSetAttribute(btnNovo, "txtSenha", (char*)txtSenha);

    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtNome", (char*)txtNome);
    IupSetAttribute(btnSalvar, "txtUser", (char*)txtUser);
    IupSetAttribute(btnSalvar, "txtSenha", (char*)txtSenha);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_oper);
    /* tamanhos padronizados aplicados por ui_pair */
    IupSetAttribute(mat_oper,"txtId",(char*)txtId); IupSetAttribute(mat_oper,"txtNome",(char*)txtNome); IupSetAttribute(mat_oper,"txtUser",(char*)txtUser); IupSetAttribute(mat_oper,"txtSenha",(char*)txtSenha);

    IupSetCallback(btnNovo, "ACTION", (Icallback)operador_novo_cb);
    IupSetCallback(btnSalvar, "ACTION", (Icallback)operador_salvar_wrap_cb);
    IupSetCallback(btnExcluir, "ACTION", (Icallback)operador_excluir_wrap_cb);
    IupSetCallback(btnAtualizar, "ACTION", (Icallback)operador_atualizar_cb);
    IupSetCallback(mat_oper, "CLICK_CB", (Icallback)operador_click_cb);

    operador_recarregar_matriz(mat_oper);

    return form;
}

void operador_view_mostrar(void) {
    if (!dlg_operador) {
        dlg_operador = IupDialog(operador_view_create());
        IupSetAttribute(dlg_operador, "TITLE", "Cadastro de Operadores");
        IupSetAttribute(dlg_operador, "SIZE", "640x400");
        IupSetAttribute(dlg_operador, "RESIZE", "YES");
        IupSetAttribute(dlg_operador, "MAXBOX", "YES");
    }
    IupShowXY(dlg_operador, IUP_CENTER, IUP_CENTER);
}
