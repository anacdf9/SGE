#include <iup.h>
#include <iupcontrols.h>
#include "../controller/equipe_controller.h"
#include <stdlib.h>
#include <stdio.h>

static Ihandle *dlg_equipe = NULL;
static Ihandle *mat_equipe = NULL;

static void equipe_recarregar_matriz(Ihandle *mat){
    Equipe buf[256]; int n=equipe_listar(buf,256); char k[16]; sprintf(k, "%d", n>0?n:1);
    IupSetAttribute(mat,"NUMCOL","5"); IupSetAttribute(mat,"NUMLIN",k);
    IupSetAttribute(mat,"0:1","ID"); IupSetAttribute(mat,"0:2","Nome"); IupSetAttribute(mat,"0:3","CPF"); IupSetAttribute(mat,"0:4","Função"); IupSetAttribute(mat,"0:5","Valor");
    for(int i=1;i<=n;i++){ char idb[32]; sprintf(idb,"%d",buf[i-1].id); char vbuf[32]; sprintf(vbuf,"%.2f",buf[i-1].valor_hora);
        IupSetAttributeId2(mat,"",i,1,idb);
        IupSetAttributeId2(mat,"",i,2,buf[i-1].nome);
        IupSetAttributeId2(mat,"",i,3,buf[i-1].cpf);
        IupSetAttributeId2(mat,"",i,4,buf[i-1].funcao);
        IupSetAttributeId2(mat,"",i,5,vbuf);
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
    equipe_salvar_cb(self);
    if (mat_equipe) equipe_recarregar_matriz(mat_equipe);
    return IUP_DEFAULT;
}

static int equipe_excluir_wrap_cb(Ihandle *self){
    equipe_excluir_cb(self);
    if (mat_equipe) equipe_recarregar_matriz(mat_equipe);
    return IUP_DEFAULT;
}

static Ihandle* equipe_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    Ihandle *txtNome = IupText(NULL);
    Ihandle *txtCpf = IupText(NULL);
    Ihandle *txtFunc = IupText(NULL);
    Ihandle *txtValor = IupText(NULL);

    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    mat_equipe = IupMatrix(NULL);
    IupSetAttribute(mat_equipe,"NUMCOL","5"); IupSetAttribute(mat_equipe,"NUMLIN","1"); IupSetAttribute(mat_equipe,"EXPAND","YES");
    IupSetAttribute(mat_equipe,"WIDTH1","40"); IupSetAttribute(mat_equipe,"WIDTH2","160"); IupSetAttribute(mat_equipe,"WIDTH3","120"); IupSetAttribute(mat_equipe,"WIDTH4","120"); IupSetAttribute(mat_equipe,"WIDTH5","80");

    Ihandle *rows = IupVbox(
        IupVbox(IupLabel("Código:"), txtId, NULL),
        IupVbox(IupLabel("Nome:"), txtNome, NULL),
        IupVbox(IupLabel("CPF:"), txtCpf, NULL),
        IupVbox(IupLabel("Função:"), txtFunc, NULL),
        IupVbox(IupLabel("Valor diária/hora:"), txtValor, NULL),
        NULL
    );
    IupSetAttribute(rows, "GAP", "4");

    Ihandle *form = IupVbox(
        IupLabel("Cadastro de Equipe Interna"),
        rows,
        IupHbox(btnSalvar, btnExcluir, btnAtualizar, NULL),
        IupSetAttributes(IupFrame(mat_equipe), "TITLE=Lista de Equipe"),
        NULL
    );
    IupSetAttribute(form, "MARGIN", "8x8");

    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtNome", (char*)txtNome);
    IupSetAttribute(btnSalvar, "txtCpf", (char*)txtCpf);
    IupSetAttribute(btnSalvar, "txtFunc", (char*)txtFunc);
    IupSetAttribute(btnSalvar, "txtValor", (char*)txtValor);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_equipe);
    IupSetAttribute(txtId,"SIZE","60x"); IupSetAttribute(txtNome,"SIZE","220x"); IupSetAttribute(txtCpf,"SIZE","140x"); IupSetAttribute(txtFunc,"SIZE","180x"); IupSetAttribute(txtValor,"SIZE","100x");

    IupSetAttribute(mat_equipe,"txtId",(char*)txtId); IupSetAttribute(mat_equipe,"txtNome",(char*)txtNome); IupSetAttribute(mat_equipe,"txtCpf",(char*)txtCpf); IupSetAttribute(mat_equipe,"txtFunc",(char*)txtFunc); IupSetAttribute(mat_equipe,"txtValor",(char*)txtValor);

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
        IupSetAttribute(dlg_equipe, "SIZE", "700x420");
    }
    IupShowXY(dlg_equipe, IUP_CENTER, IUP_CENTER);
}
