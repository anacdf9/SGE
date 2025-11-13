#include <iup.h>
#include <iupcontrols.h> // <-- ESSA LINHA QUE FALTAVA (ou não salvou)
#include <stdlib.h>      // (Esse aqui era pro NULL)

#include "src/view/cliente_view.h"
#include "src/config.h"
#include "src/model/pers.h"

/* Callback para o menu "Clientes" */
int btn_abrir_clientes_cb(void) {
    cliente_view_mostrar(); 
    return IUP_DEFAULT;
}

/* Callback para o menu "Sair" */
int btn_sair_cb(void) {
    return IUP_CLOSE; // Comando do IUP para fechar o MainLoop
}

int main(int argc, char **argv) {
    IupOpen(&argc, &argv);
    IupControlsOpen(); // <-- Essa função precisa do <iupcontrols.h>

    pers_inicializar(TIPO_PERSISTENCIA);

    Ihandle *menu = IupMenu(
        IupSubmenu("Cadastros", IupMenu(
            IupItem("Clientes", "btn_abrir_clientes_cb"),
            IupItem("Equipe Interna", NULL), 
            IupItem("Recursos/Equipamentos", NULL),
            IupItem("Fornecedores/Parceiros", NULL),
            IupSeparator(),
            IupItem("Sair", "btn_sair_cb")
            ,NULL
        )),
        IupSubmenu("Eventos", IupMenu(
            IupItem("Novo Orçamento", NULL),
            IupItem("Gerenciar Eventos", NULL),
            NULL
        )),
        NULL
    );

    // CRIA A JANELA PRINCIPAL
    Ihandle *dlg_main = IupDialog(
        IupVbox(NULL) // Janela vazia
    );
    IupSetAttribute(dlg_main, "TITLE", "SGE (Sistema de Gestão de Eventos)");
    IupSetAttribute(dlg_main, "SIZE", "800x600");
    
    // Associa o menu à janela principal
    IupSetAttributeHandle(dlg_main, "MENU", menu);
    
    // Registra os callbacks (só precisa registrar 1x)
    IupSetFunction("btn_abrir_clientes_cb", (Icallback)btn_abrir_clientes_cb);
    IupSetFunction("btn_sair_cb", (Icallback)btn_sair_cb);

    IupShowXY(dlg_main, IUP_CENTER, IUP_CENTER);
    IupMainLoop();
    IupClose();
    return 0;
}