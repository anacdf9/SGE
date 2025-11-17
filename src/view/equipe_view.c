#include <iup.h>
#include <iupcontrols.h>
#include "../controller/equipe_controller.h"
#include "ui_common.h"
#include "validation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static Ihandle *dlg_equipe = NULL;
static Ihandle *mat_equipe = NULL;

static void equipe_recarregar_matriz(Ihandle *mat){
    Equipe buf[256]; int n=equipe_listar(buf,256); char k[16]; sprintf(k, "%d", n>0?n:1);
    IupSetAttribute(mat,"NUMCOL","5"); IupSetAttribute(mat,"NUMLIN",k);
    IupSetAttribute(mat,"0:1","ID"); IupSetAttribute(mat,"0:2","Nome"); IupSetAttribute(mat,"0:3","CPF"); IupSetAttribute(mat,"0:4","Função"); IupSetAttribute(mat,"0:5","Valor");
    for(int i=1;i<=n;i++){ char idb[32]; sprintf(idb,"%d",buf[i-1].id); char vbuf[32]; sprintf(vbuf,"%.2f",buf[i-1].valor_hora);
        IupSetStrAttributeId2(mat,"",i,1,idb);
        IupSetStrAttributeId2(mat,"",i,2,buf[i-1].nome);
        IupSetStrAttributeId2(mat,"",i,3,buf[i-1].cpf);
        IupSetStrAttributeId2(mat,"",i,4,buf[i-1].funcao);
        IupSetStrAttributeId2(mat,"",i,5,vbuf);
    }
}

static int equipe_click_cb(Ihandle *mat,int lin,int col,char *status){ if(lin<=0) return IUP_DEFAULT;
    Ihandle *tId=(Ihandle*)IupGetAttribute(mat,"txtId"); Ihandle *tNome=(Ihandle*)IupGetAttribute(mat,"txtNome"); Ihandle *tCpf=(Ihandle*)IupGetAttribute(mat,"txtCpf"); Ihandle *tFunc=(Ihandle*)IupGetAttribute(mat,"txtFunc"); Ihandle *tValor=(Ihandle*)IupGetAttribute(mat,"txtValor");
    const char *v; v=IupGetAttributeId2(mat,"",lin,1); IupSetStrAttribute(tId,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,2); IupSetStrAttribute(tNome,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,3); IupSetStrAttribute(tCpf,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,4); IupSetStrAttribute(tFunc,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,5); IupSetStrAttribute(tValor,"VALUE",v?v:"");
    return IUP_DEFAULT; }

static int equipe_atualizar_cb(Ihandle *self){ Ihandle *m=(Ihandle*)IupGetAttribute(self,"MATRIX"); equipe_recarregar_matriz(m); return IUP_DEFAULT; }

static int equipe_salvar_wrap_cb(Ihandle *self){
    Ihandle *txtCpf = (Ihandle*)IupGetAttribute(self, "txtCpf");
    const char* s = IupGetAttribute(txtCpf, "VALUE");
    /* aceita apenas CPF: 11 dígitos */
    if (!ui_valid_cpf_cnpj(s) || (s && s[0] && (int)strlen(s) != 11)) {
        IupMessage("Dados inválidos", "CPF deve conter 11 dígitos (apenas números).");
        return IUP_DEFAULT;
    }
    equipe_salvar_cb(self);
    if (mat_equipe) equipe_recarregar_matriz(mat_equipe);
    return IUP_DEFAULT;
}

static int equipe_excluir_wrap_cb(Ihandle *self){
    equipe_excluir_cb(self);
    if (mat_equipe) equipe_recarregar_matriz(mat_equipe);
    return IUP_DEFAULT;
}

static int equipe_novo_cb(Ihandle *self){
    Ihandle *txtId=(Ihandle*)IupGetAttribute(self,"txtId");
    Ihandle *txtNome=(Ihandle*)IupGetAttribute(self,"txtNome");
    Ihandle *txtCpf=(Ihandle*)IupGetAttribute(self,"txtCpf");
    Ihandle *txtFunc=(Ihandle*)IupGetAttribute(self,"txtFunc");
    Ihandle *txtValor=(Ihandle*)IupGetAttribute(self,"txtValor");
    IupSetAttribute(txtId,"VALUE","");
    IupSetAttribute(txtNome,"VALUE","");
    IupSetAttribute(txtCpf,"VALUE","");
    IupSetAttribute(txtFunc,"VALUE","");
    IupSetAttribute(txtValor,"VALUE","");
    IupSetFocus(txtNome);
    return IUP_DEFAULT;
}

static Ihandle* equipe_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    IupSetAttribute(txtId, "READONLY", "YES");
    IupSetAttribute(txtId, "TIP", "Gerado automaticamente");
    Ihandle *txtNome = IupText(NULL);
    Ihandle *txtCpf = IupText(NULL);
    Ihandle *txtFunc = IupText(NULL);
    Ihandle *txtValor = IupText(NULL);

    Ihandle *btnNovo = IupButton("Novo", NULL);
    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    mat_equipe = IupMatrix(NULL);
    IupSetAttribute(mat_equipe,"NUMCOL","5"); IupSetAttribute(mat_equipe,"NUMLIN","1"); IupSetAttribute(mat_equipe,"EXPAND","YES");
    IupSetAttribute(mat_equipe,"WIDTH1","40"); IupSetAttribute(mat_equipe,"WIDTH2","160"); IupSetAttribute(mat_equipe,"WIDTH3","120"); IupSetAttribute(mat_equipe,"WIDTH4","120"); IupSetAttribute(mat_equipe,"WIDTH5","80");

    ui_set_width_px(txtId, UI_W_ID);
    ui_set_width_px(txtCpf, UI_W_MED);
    ui_set_width_px(txtNome, UI_W_XLONG);
    ui_set_width_px(txtFunc, UI_W_LONG);
    ui_set_width_px(txtValor, UI_W_SHORT);
    IupSetAttribute(txtCpf, "TIP", "CPF: 11 dígitos (somente números)");

    Ihandle *rows = IupVbox(
        ui_pair("Código:", txtId),
        ui_pair("CPF:", txtCpf),
        ui_pair("Nome:", txtNome),
        ui_pair("Função:", txtFunc),
        ui_pair("Valor diária/hora:", txtValor),
        NULL
    );
    ui_style_form(rows);

    Ihandle *btn_row = IupHbox(IupFill(), btnNovo, btnSalvar, btnExcluir, btnAtualizar, IupFill(), NULL);
    Ihandle *form = IupVbox(
        IupLabel("Cadastro de Equipe Interna"),
        rows,
        btn_row,
        IupSetAttributes(IupFrame(mat_equipe), "TITLE=Lista de Equipe"),
        NULL
    );
    ui_style_form(form);

    IupSetAttribute(btnNovo, "txtId", (char*)txtId);
    IupSetAttribute(btnNovo, "txtNome", (char*)txtNome);
    IupSetAttribute(btnNovo, "txtCpf", (char*)txtCpf);
    IupSetAttribute(btnNovo, "txtFunc", (char*)txtFunc);
    IupSetAttribute(btnNovo, "txtValor", (char*)txtValor);

    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtNome", (char*)txtNome);
    IupSetAttribute(btnSalvar, "txtCpf", (char*)txtCpf);
    IupSetAttribute(btnSalvar, "txtFunc", (char*)txtFunc);
    IupSetAttribute(btnSalvar, "txtValor", (char*)txtValor);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_equipe);
    /* tamanhos padronizados aplicados por ui_pair */

    IupSetAttribute(mat_equipe,"txtId",(char*)txtId); IupSetAttribute(mat_equipe,"txtNome",(char*)txtNome); IupSetAttribute(mat_equipe,"txtCpf",(char*)txtCpf); IupSetAttribute(mat_equipe,"txtFunc",(char*)txtFunc); IupSetAttribute(mat_equipe,"txtValor",(char*)txtValor);

    IupSetCallback(btnNovo, "ACTION", (Icallback)equipe_novo_cb);
    IupSetCallback(btnSalvar, "ACTION", (Icallback)equipe_salvar_wrap_cb);
    IupSetCallback(btnExcluir, "ACTION", (Icallback)equipe_excluir_wrap_cb);
    IupSetCallback(btnAtualizar, "ACTION", (Icallback)equipe_atualizar_cb);
    IupSetCallback(mat_equipe, "CLICK_CB", (Icallback)equipe_click_cb);

    equipe_recarregar_matriz(mat_equipe);

    return form;
}

void equipe_view_mostrar(void) {
    if (!dlg_equipe) {
        dlg_equipe = IupDialog(equipe_view_create());
        IupSetAttribute(dlg_equipe, "TITLE", "Cadastro de Equipe");
        IupSetAttribute(dlg_equipe, "SIZE", "720x520");
    }
    IupShowXY(dlg_equipe, IUP_CENTER, IUP_CENTER);
}
