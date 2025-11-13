#include <iup.h>
#include <iupcontrols.h>
#include "../controller/produtora_controller.h"
#include <stdlib.h>

static Ihandle *dlg_produtora = NULL;

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

    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnCarregar = IupButton("Carregar", NULL);

    /* Padrão: rótulo em cima do input, espaçamento compacto */
    IupSetAttribute(tNF, "SIZE", "220x");
    IupSetAttribute(tRS, "SIZE", "220x");
    IupSetAttribute(tIE, "SIZE", "220x");
    IupSetAttribute(tCNPJ, "SIZE", "220x");
    IupSetAttribute(tEnd, "SIZE", "220x");
    IupSetAttribute(tTel, "SIZE", "220x");
    IupSetAttribute(tEmail, "SIZE", "220x");
    IupSetAttribute(tResp, "SIZE", "220x");
    IupSetAttribute(tTelResp, "SIZE", "220x");
    IupSetAttribute(tMargem, "SIZE", "220x");

    Ihandle *rows = IupVbox(
        IupVbox(IupLabel("Nome Fantasia:"), tNF, NULL),
        IupVbox(IupLabel("Razão Social:"), tRS, NULL),
        IupVbox(IupLabel("Inscrição Estadual:"), tIE, NULL),
        IupVbox(IupLabel("CNPJ:"), tCNPJ, NULL),
        IupVbox(IupLabel("Endereço:"), tEnd, NULL),
        IupVbox(IupLabel("Telefone:"), tTel, NULL),
        IupVbox(IupLabel("E-mail:"), tEmail, NULL),
        IupVbox(IupLabel("Responsável:"), tResp, NULL),
        IupVbox(IupLabel("Tel. Responsável:"), tTelResp, NULL),
        IupVbox(IupLabel("Margem Lucro (%):"), tMargem, NULL),
        NULL
    );
    IupSetAttribute(rows, "GAP", "4");

    Ihandle *form = IupVbox(
        IupLabel("Dados da Produtora"),
        rows,
        IupHbox(btnSalvar, btnCarregar, NULL),
        NULL
    );
    IupSetAttribute(form, "MARGIN", "8x8");

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

    IupSetCallback(btnSalvar, "ACTION", (Icallback)produtora_salvar_cb);
    IupSetCallback(btnCarregar, "ACTION", (Icallback)produtora_carregar_cb);

    return form;
}

void produtora_view_mostrar(void) {
    if (!dlg_produtora) {
        dlg_produtora = IupDialog(produtora_view_create());
        IupSetAttribute(dlg_produtora, "TITLE", "Produtora de Eventos");
        IupSetAttribute(dlg_produtora, "SIZE", "460x320");
    }
    IupShowXY(dlg_produtora, IUP_CENTER, IUP_CENTER);
}
