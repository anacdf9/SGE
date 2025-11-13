#include <stdio.h>
#include <iup.h>
#include "src/config.h"
#include "src/model/pers.h"
#include "src/view/cliente_view.h"
#include "iupcontrols.h"

static Ihandle *dlgMain;

static int btn_abrir_clientes_cb(Ihandle *self) {
    cliente_view_mostrar();
    return IUP_DEFAULT;
}

static int btn_sair_cb(Ihandle *self) {
    IupExitLoop();
    return IUP_CLOSE;
}

int main(int argc, char **argv) {
    IupOpen(&argc, &argv);
    IupSetGlobal("UTF8MODE", "YES"); /* ativar acentuação no IUP */

    /* inicializa persistência conforme config (TIPO_PERSISTENCIA definido em src/config.h) */
    pers_inicializar(TIPO_PERSISTENCIA);

    /* Interface principal simples */
    IupSetLanguage(IUP_ENGLISH); /* mantém idioma neutro */

    Ihandle *btnClientes = IupButton("Clientes", NULL);
    Ihandle *btnSair = IupButton("Sair", NULL);
    IupSetCallback(btnClientes, "ACTION", (Icallback)btn_abrir_clientes_cb);
    IupSetCallback(btnSair, "ACTION", (Icallback)btn_sair_cb);
    IupSetAttribute(btnClientes, "SIZE", "200x40");
    IupSetAttribute(btnSair, "SIZE", "200x40");

    Ihandle *v = IupVbox(
        IupLabel("Sistema de Gestão de Eventos (SGE)"),
        IupFill(),
        btnClientes,
        IupFill(),
        btnSair,
        NULL
    );
    IupSetAttribute(v, "GAP", "10");
    IupSetAttribute(v, "MARGIN", "20x20");
    IupSetAttribute(v, "ALIGNMENT", "ACENTER");

    dlgMain = IupDialog(v);
    IupSetAttribute(dlgMain, "TITLE", "SGE - Menu Principal");
    IupSetAttribute(dlgMain, "SIZE", "400x300");

    IupShowXY(dlgMain, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    /* quando sair do loop principal, finaliza persistência e IUP */
    pers_finalizar();
    IupClose();
    return 0;
}
