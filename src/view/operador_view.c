#include <iup.h>
#include <iupcontrols.h>
#include "../controller/operador_controller.h"
#include <stdlib.h>
#include <stdio.h>

static Ihandle *dlg_operador = NULL;
static Ihandle *mat_oper = NULL;

static void operador_recarregar_matriz(Ihandle *mat){ Operador buf[256]; int n=operador_listar(buf,256); char k[16]; sprintf(k,"%d",n>0?n:1); IupSetAttribute(mat,"NUMCOL","4"); IupSetAttribute(mat,"NUMLIN",k); IupSetAttribute(mat,"0:1","ID"); IupSetAttribute(mat,"0:2","Nome"); IupSetAttribute(mat,"0:3","Usuário"); IupSetAttribute(mat,"0:4","Senha"); for(int i=1;i<=n;i++){ char idb[32]; sprintf(idb,"%d",buf[i-1].id); IupSetAttributeId2(mat,"",i,1,idb); IupSetAttributeId2(mat,"",i,2,buf[i-1].nome); IupSetAttributeId2(mat,"",i,3,buf[i-1].usuario); IupSetAttributeId2(mat,"",i,4,buf[i-1].senha);} }
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

static Ihandle* operador_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    Ihandle *txtNome = IupText(NULL);
    Ihandle *txtUser = IupText(NULL);
    Ihandle *txtSenha = IupText(NULL);
    IupSetAttribute(txtSenha, "PASSWORD", "YES");

    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    mat_oper = IupMatrix(NULL);
    IupSetAttribute(mat_oper,"NUMCOL","4"); IupSetAttribute(mat_oper,"NUMLIN","1"); IupSetAttribute(mat_oper,"EXPAND","YES");
    IupSetAttribute(mat_oper,"WIDTH1","40"); IupSetAttribute(mat_oper,"WIDTH2","160"); IupSetAttribute(mat_oper,"WIDTH3","140"); IupSetAttribute(mat_oper,"WIDTH4","120");

    Ihandle *rows = IupVbox(
        IupVbox(IupLabel("Código:"), txtId, NULL),
        IupVbox(IupLabel("Nome:"), txtNome, NULL),
        IupVbox(IupLabel("Usuário:"), txtUser, NULL),
        IupVbox(IupLabel("Senha:"), txtSenha, NULL),
        NULL
    );
    IupSetAttribute(rows, "GAP", "4");

    Ihandle *form = IupVbox(
        IupLabel("Cadastro de Operadores do Sistema"),
        rows,
        IupHbox(btnSalvar, btnExcluir, btnAtualizar, NULL),
        IupSetAttributes(IupFrame(mat_oper), "TITLE=Lista de Operadores"),
        NULL
    );
    IupSetAttribute(form, "MARGIN", "8x8");

    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtNome", (char*)txtNome);
    IupSetAttribute(btnSalvar, "txtUser", (char*)txtUser);
    IupSetAttribute(btnSalvar, "txtSenha", (char*)txtSenha);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_oper);
    IupSetAttribute(txtId,"SIZE","60x"); IupSetAttribute(txtNome,"SIZE","220x"); IupSetAttribute(txtUser,"SIZE","160x"); IupSetAttribute(txtSenha,"SIZE","160x");
    IupSetAttribute(mat_oper,"txtId",(char*)txtId); IupSetAttribute(mat_oper,"txtNome",(char*)txtNome); IupSetAttribute(mat_oper,"txtUser",(char*)txtUser); IupSetAttribute(mat_oper,"txtSenha",(char*)txtSenha);

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
        IupSetAttribute(dlg_operador, "SIZE", "700x380");
    }
    IupShowXY(dlg_operador, IUP_CENTER, IUP_CENTER);
}
