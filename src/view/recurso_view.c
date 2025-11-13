#include <iup.h>
#include <iupcontrols.h>
#include "../controller/recurso_controller.h"
#include <stdlib.h>
#include <stdio.h>

static Ihandle *dlg_recurso = NULL;
static Ihandle *mat_recurso = NULL;

static void recurso_recarregar_matriz(Ihandle *mat){
    Recurso buffer[256]; int n=recurso_listar(buffer,256); char numlin[16]; sprintf(numlin, "%d", n>0?n:1);
    IupSetAttribute(mat, "NUMCOL", "6"); IupSetAttribute(mat, "NUMLIN", numlin);
    IupSetAttribute(mat, "0:1", "ID"); IupSetAttribute(mat, "0:2", "Descrição"); IupSetAttribute(mat, "0:3", "Categoria");
    IupSetAttribute(mat, "0:4", "Qtd"); IupSetAttribute(mat, "0:5", "Custo"); IupSetAttribute(mat, "0:6", "Locação");
    for(int i=1;i<=n;i++){ char idbuf[32]; sprintf(idbuf,"%d",buffer[i-1].id); char qbuf[32]; sprintf(qbuf,"%d",buffer[i-1].quantidade); char cbuf[32]; sprintf(cbuf,"%.2f",buffer[i-1].preco_custo); char lbuf[32]; sprintf(lbuf,"%.2f",buffer[i-1].valor_locacao);
        IupSetAttributeId2(mat,"",i,1,idbuf);
        IupSetAttributeId2(mat,"",i,2,buffer[i-1].descricao);
        IupSetAttributeId2(mat,"",i,3,buffer[i-1].categoria);
        IupSetAttributeId2(mat,"",i,4,qbuf);
        IupSetAttributeId2(mat,"",i,5,cbuf);
        IupSetAttributeId2(mat,"",i,6,lbuf);
    }
}

static int recurso_click_cb(Ihandle *mat,int lin,int col,char *status){ if(lin<=0) return IUP_DEFAULT;
    Ihandle *tId=(Ihandle*)IupGetAttribute(mat,"txtId"); Ihandle *tDesc=(Ihandle*)IupGetAttribute(mat,"txtDesc"); Ihandle *tCat=(Ihandle*)IupGetAttribute(mat,"txtCat");
    Ihandle *tQtd=(Ihandle*)IupGetAttribute(mat,"txtQtd"); Ihandle *tCusto=(Ihandle*)IupGetAttribute(mat,"txtCusto"); Ihandle *tLoc=(Ihandle*)IupGetAttribute(mat,"txtLoc");
    const char *v; v=IupGetAttributeId2(mat,"",lin,1); IupSetStrAttribute(tId,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,2); IupSetStrAttribute(tDesc,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,3); IupSetStrAttribute(tCat,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,4); IupSetStrAttribute(tQtd,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,5); IupSetStrAttribute(tCusto,"VALUE",v?v:"");
    v=IupGetAttributeId2(mat,"",lin,6); IupSetStrAttribute(tLoc,"VALUE",v?v:"");
    return IUP_DEFAULT; }

static int recurso_atualizar_cb(Ihandle *self){ Ihandle *m=(Ihandle*)IupGetAttribute(self,"MATRIX"); recurso_recarregar_matriz(m); return IUP_DEFAULT; }

static int recurso_salvar_wrap_cb(Ihandle *self){
    recurso_salvar_cb(self);
    if (mat_recurso) recurso_recarregar_matriz(mat_recurso);
    return IUP_DEFAULT;
}

static int recurso_excluir_wrap_cb(Ihandle *self){
    recurso_excluir_cb(self);
    if (mat_recurso) recurso_recarregar_matriz(mat_recurso);
    return IUP_DEFAULT;
}

static Ihandle* recurso_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    Ihandle *txtDesc = IupText(NULL);
    Ihandle *txtCat = IupText(NULL);
    Ihandle *txtQtd = IupText(NULL);
    Ihandle *txtCusto = IupText(NULL);
    Ihandle *txtLoc = IupText(NULL);

    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    mat_recurso = IupMatrix(NULL);
    IupSetAttribute(mat_recurso,"NUMCOL","6"); IupSetAttribute(mat_recurso,"NUMLIN","1"); IupSetAttribute(mat_recurso,"EXPAND","YES");
    IupSetAttribute(mat_recurso,"WIDTH1","40"); IupSetAttribute(mat_recurso,"WIDTH2","160"); IupSetAttribute(mat_recurso,"WIDTH3","120");
    IupSetAttribute(mat_recurso,"WIDTH4","50"); IupSetAttribute(mat_recurso,"WIDTH5","80"); IupSetAttribute(mat_recurso,"WIDTH6","80");

    Ihandle *rows = IupVbox(
        IupVbox(IupLabel("Código:"), txtId, NULL),
        IupVbox(IupLabel("Descrição:"), txtDesc, NULL),
        IupVbox(IupLabel("Categoria:"), txtCat, NULL),
        IupVbox(IupLabel("Quantidade:"), txtQtd, NULL),
        IupVbox(IupLabel("Preço de Custo:"), txtCusto, NULL),
        IupVbox(IupLabel("Valor da Locação:"), txtLoc, NULL),
        NULL
    );
    IupSetAttribute(rows, "GAP", "4");

    Ihandle *form = IupVbox(
        IupLabel("Cadastro de Recursos/Equipamentos"),
        rows,
        IupHbox(btnSalvar, btnExcluir, btnAtualizar, NULL),
        IupSetAttributes(IupFrame(mat_recurso), "TITLE=Lista de Recursos"),
        NULL
    );
    IupSetAttribute(form, "MARGIN", "8x8");

    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtDesc", (char*)txtDesc);
    IupSetAttribute(btnSalvar, "txtCat", (char*)txtCat);
    IupSetAttribute(btnSalvar, "txtQtd", (char*)txtQtd);
    IupSetAttribute(btnSalvar, "txtCusto", (char*)txtCusto);
    IupSetAttribute(btnSalvar, "txtLoc", (char*)txtLoc);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_recurso);
    IupSetAttribute(txtId,"SIZE","60x"); IupSetAttribute(txtDesc,"SIZE","220x"); IupSetAttribute(txtCat,"SIZE","160x");
    IupSetAttribute(txtQtd,"SIZE","60x"); IupSetAttribute(txtCusto,"SIZE","100x"); IupSetAttribute(txtLoc,"SIZE","100x");

    IupSetAttribute(mat_recurso,"txtId",(char*)txtId); IupSetAttribute(mat_recurso,"txtDesc",(char*)txtDesc);
    IupSetAttribute(mat_recurso,"txtCat",(char*)txtCat); IupSetAttribute(mat_recurso,"txtQtd",(char*)txtQtd);
    IupSetAttribute(mat_recurso,"txtCusto",(char*)txtCusto); IupSetAttribute(mat_recurso,"txtLoc",(char*)txtLoc);

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
        IupSetAttribute(dlg_recurso, "SIZE", "720x420");
    }
    IupShowXY(dlg_recurso, IUP_CENTER, IUP_CENTER);
}
