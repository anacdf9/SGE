#include <iup.h>
#include <iupcontrols.h>
#include "../controller/fornecedor_controller.h"
#include "ui_common.h"
#include "validation.h"
#include <stdlib.h>
#include <stdio.h>

static Ihandle *dlg_fornecedor = NULL;
static Ihandle *mat_forn = NULL;

static void fornecedor_recarregar_matriz(Ihandle *mat){
    Fornecedor buf[256]; int n=fornecedor_listar(buf,256); char k[16]; sprintf(k,"%d",n>0?n:1);
    IupSetAttribute(mat,"NUMCOL","7"); IupSetAttribute(mat,"NUMLIN",k);
    IupSetAttribute(mat,"0:1","ID"); IupSetAttribute(mat,"0:2","Nome Fantasia"); IupSetAttribute(mat,"0:3","Razão Social"); IupSetAttribute(mat,"0:4","CNPJ/CPF"); IupSetAttribute(mat,"0:5","Endereço"); IupSetAttribute(mat,"0:6","Telefone"); IupSetAttribute(mat,"0:7","Serviço");
    for(int i=1;i<=n;i++){ char idb[32]; sprintf(idb,"%d",buf[i-1].id);
        IupSetStrAttributeId2(mat,"",i,1,idb);
        IupSetStrAttributeId2(mat,"",i,2,buf[i-1].nome_fantasia);
        IupSetStrAttributeId2(mat,"",i,3,buf[i-1].razao_social);
        IupSetStrAttributeId2(mat,"",i,4,buf[i-1].cnpj_cpf);
        IupSetStrAttributeId2(mat,"",i,5,buf[i-1].endereco);
        IupSetStrAttributeId2(mat,"",i,6,buf[i-1].telefone);
        IupSetStrAttributeId2(mat,"",i,7,buf[i-1].tipo_servico);
    }
}

static int fornecedor_click_cb(Ihandle *mat,int lin,int col,char *status){ if(lin<=0) return IUP_DEFAULT; Ihandle *tId=(Ihandle*)IupGetAttribute(mat,"txtId"); Ihandle *tNF=(Ihandle*)IupGetAttribute(mat,"txtNF"); Ihandle *tRS=(Ihandle*)IupGetAttribute(mat,"txtRS"); Ihandle *tDoc=(Ihandle*)IupGetAttribute(mat,"txtDoc"); Ihandle *tEnd=(Ihandle*)IupGetAttribute(mat,"txtEnd"); Ihandle *tTel=(Ihandle*)IupGetAttribute(mat,"txtTel"); Ihandle *tServ=(Ihandle*)IupGetAttribute(mat,"txtServ"); const char *v; v=IupGetAttributeId2(mat,"",lin,1); IupSetStrAttribute(tId,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,2); IupSetStrAttribute(tNF,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,3); IupSetStrAttribute(tRS,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,4); IupSetStrAttribute(tDoc,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,5); IupSetStrAttribute(tEnd,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,6); IupSetStrAttribute(tTel,"VALUE",v?v:""); v=IupGetAttributeId2(mat,"",lin,7); IupSetStrAttribute(tServ,"VALUE",v?v:""); return IUP_DEFAULT; }

static int fornecedor_atualizar_cb(Ihandle *self){ Ihandle *m=(Ihandle*)IupGetAttribute(self,"MATRIX"); fornecedor_recarregar_matriz(m); return IUP_DEFAULT; }

static int fornecedor_salvar_wrap_cb(Ihandle *self){
    Ihandle *txtDoc = (Ihandle*)IupGetAttribute(self, "txtDoc");
    Ihandle *txtTel = (Ihandle*)IupGetAttribute(self, "txtTel");
    const char* doc = IupGetAttribute(txtDoc, "VALUE");
    const char* tel = IupGetAttribute(txtTel, "VALUE");
    if (!ui_valid_cpf_cnpj(doc)) { IupMessage("Dados inválidos", "Documento deve ser CPF(11) ou CNPJ(14) somente com dígitos."); return IUP_DEFAULT; }
    if (!ui_valid_telefone(tel)) { IupMessage("Dados inválidos", "Telefone deve conter 10 ou 11 dígitos."); return IUP_DEFAULT; }
    fornecedor_salvar_cb(self);
    if (mat_forn) fornecedor_recarregar_matriz(mat_forn);
    return IUP_DEFAULT;
}

static int fornecedor_excluir_wrap_cb(Ihandle *self){
    fornecedor_excluir_cb(self);
    if (mat_forn) fornecedor_recarregar_matriz(mat_forn);
    return IUP_DEFAULT;
}

static int fornecedor_novo_cb(Ihandle *self){
    Ihandle *txtId=(Ihandle*)IupGetAttribute(self,"txtId");
    Ihandle *txtNF=(Ihandle*)IupGetAttribute(self,"txtNF");
    Ihandle *txtRS=(Ihandle*)IupGetAttribute(self,"txtRS");
    Ihandle *txtDoc=(Ihandle*)IupGetAttribute(self,"txtDoc");
    Ihandle *txtEnd=(Ihandle*)IupGetAttribute(self,"txtEnd");
    Ihandle *txtTel=(Ihandle*)IupGetAttribute(self,"txtTel");
    Ihandle *txtServ=(Ihandle*)IupGetAttribute(self,"txtServ");
    IupSetAttribute(txtId,"VALUE","");
    IupSetAttribute(txtNF,"VALUE","");
    IupSetAttribute(txtRS,"VALUE","");
    IupSetAttribute(txtDoc,"VALUE","");
    IupSetAttribute(txtEnd,"VALUE","");
    IupSetAttribute(txtTel,"VALUE","");
    IupSetAttribute(txtServ,"VALUE","");
    IupSetFocus(txtNF);
    return IUP_DEFAULT;
}

static Ihandle* fornecedor_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    IupSetAttribute(txtId, "READONLY", "YES");
    IupSetAttribute(txtId, "TIP", "Gerado automaticamente");
    Ihandle *txtNF = IupText(NULL);
    Ihandle *txtRS = IupText(NULL);
    Ihandle *txtDoc = IupText(NULL);
    Ihandle *txtEnd = IupText(NULL);
    Ihandle *txtTel = IupText(NULL);
    Ihandle *txtServ = IupText(NULL);

    Ihandle *btnNovo = IupButton("Novo", NULL);
    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnAtualizar = IupButton("Atualizar Lista", NULL);

    mat_forn = IupMatrix(NULL);
    IupSetAttribute(mat_forn,"NUMCOL","7"); IupSetAttribute(mat_forn,"NUMLIN","1"); IupSetAttribute(mat_forn,"EXPAND","YES");
    IupSetAttribute(mat_forn,"WIDTH1","40"); IupSetAttribute(mat_forn,"WIDTH2","140"); IupSetAttribute(mat_forn,"WIDTH3","160"); IupSetAttribute(mat_forn,"WIDTH4","100"); IupSetAttribute(mat_forn,"WIDTH5","200"); IupSetAttribute(mat_forn,"WIDTH6","100"); IupSetAttribute(mat_forn,"WIDTH7","120");

    ui_set_width_px(txtId, UI_W_ID);
    ui_set_width_px(txtDoc, UI_W_MED);
    ui_set_width_px(txtNF, UI_W_XLONG);
    ui_set_width_px(txtRS, UI_W_XLONG);
    ui_set_width_px(txtEnd, UI_W_XLONG);
    ui_set_width_px(txtTel, UI_W_MED);
    ui_set_width_px(txtServ, UI_W_LONG);
    IupSetAttribute(txtDoc, "TIP", "Somente dígitos: CPF=11, CNPJ=14");
    IupSetAttribute(txtTel, "TIP", "Somente dígitos: Tel=10-11");

    Ihandle *rows = IupVbox(
        ui_pair("Código:", txtId),
        ui_pair("CNPJ/CPF:", txtDoc),
        ui_pair("Nome Fantasia:", txtNF),
        ui_pair("Razão Social:", txtRS),
        ui_pair("Endereço:", txtEnd),
        ui_pair("Telefone:", txtTel),
        ui_pair("Tipo de Serviço:", txtServ),
        NULL
    );
    ui_style_form(rows);

    Ihandle *btn_row = IupHbox(IupFill(), btnNovo, btnSalvar, btnExcluir, btnAtualizar, IupFill(), NULL);
    Ihandle *form = IupVbox(
        IupLabel("Cadastro de Fornecedores/Parceiros"),
        rows,
        btn_row,
        IupSetAttributes(IupFrame(mat_forn), "TITLE=Lista de Fornecedores"),
        NULL
    );
    ui_style_form(form);

    IupSetAttribute(btnNovo, "txtId", (char*)txtId);
    IupSetAttribute(btnNovo, "txtNF", (char*)txtNF);
    IupSetAttribute(btnNovo, "txtRS", (char*)txtRS);
    IupSetAttribute(btnNovo, "txtDoc", (char*)txtDoc);
    IupSetAttribute(btnNovo, "txtEnd", (char*)txtEnd);
    IupSetAttribute(btnNovo, "txtTel", (char*)txtTel);
    IupSetAttribute(btnNovo, "txtServ", (char*)txtServ);

    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtNF", (char*)txtNF);
    IupSetAttribute(btnSalvar, "txtRS", (char*)txtRS);
    IupSetAttribute(btnSalvar, "txtDoc", (char*)txtDoc);
    IupSetAttribute(btnSalvar, "txtEnd", (char*)txtEnd);
    IupSetAttribute(btnSalvar, "txtTel", (char*)txtTel);
    IupSetAttribute(btnSalvar, "txtServ", (char*)txtServ);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);
    IupSetAttribute(btnAtualizar, "MATRIX", (char*)mat_forn);
    /* tamanhos padronizados aplicados por ui_pair */

    IupSetAttribute(mat_forn,"txtId",(char*)txtId); IupSetAttribute(mat_forn,"txtNF",(char*)txtNF); IupSetAttribute(mat_forn,"txtRS",(char*)txtRS); IupSetAttribute(mat_forn,"txtDoc",(char*)txtDoc); IupSetAttribute(mat_forn,"txtEnd",(char*)txtEnd); IupSetAttribute(mat_forn,"txtTel",(char*)txtTel); IupSetAttribute(mat_forn,"txtServ",(char*)txtServ);

    IupSetCallback(btnNovo, "ACTION", (Icallback)fornecedor_novo_cb);
    IupSetCallback(btnSalvar, "ACTION", (Icallback)fornecedor_salvar_wrap_cb);
    IupSetCallback(btnExcluir, "ACTION", (Icallback)fornecedor_excluir_wrap_cb);
    IupSetCallback(btnAtualizar, "ACTION", (Icallback)fornecedor_atualizar_cb);
    IupSetCallback(mat_forn, "CLICK_CB", (Icallback)fornecedor_click_cb);

    fornecedor_recarregar_matriz(mat_forn);

    return form;
}

void fornecedor_view_mostrar(void) {
    if (!dlg_fornecedor) {
        dlg_fornecedor = IupDialog(fornecedor_view_create());
        IupSetAttribute(dlg_fornecedor, "TITLE", "Cadastro de Fornecedores");
        IupSetAttribute(dlg_fornecedor, "SIZE", "760x540");
        IupSetAttribute(dlg_fornecedor, "RESIZE", "YES");
        IupSetAttribute(dlg_fornecedor, "MAXBOX", "YES");
    }
    IupShowXY(dlg_fornecedor, IUP_CENTER, IUP_CENTER);
}
