#include <iup.h>
#include <iupcontrols.h> // <-- ESSA LINHA QUE FALTAVA (ou não salvou)
#include <stdlib.h>      // (Esse aqui era pro NULL)

#include "src/view/cliente_view.h"
#include "src/view/equipe_view.h"
#include "src/view/recurso_view.h"
#include "src/view/fornecedor_view.h"
#include "src/view/operador_view.h"
#include "src/view/produtora_view.h"
#include "src/config.h"
#include "src/model/pers.h"

/* Callback para o menu "Clientes" */
int btn_abrir_clientes_cb(void) {
    cliente_view_mostrar(); 
    return IUP_DEFAULT;
}

int btn_abrir_equipe_cb(void) {
    equipe_view_mostrar();
    return IUP_DEFAULT;
}

int btn_abrir_recursos_cb(void) {
    recurso_view_mostrar();
    return IUP_DEFAULT;
}

int btn_abrir_fornecedores_cb(void) {
    fornecedor_view_mostrar();
    return IUP_DEFAULT;
}

int btn_abrir_operadores_cb(void) {
    operador_view_mostrar();
    return IUP_DEFAULT;
}

int btn_abrir_produtora_cb(void) {
    produtora_view_mostrar();
    return IUP_DEFAULT;
}

/* Callback para o menu "Sair" */
int btn_sair_cb(void) {
    return IUP_CLOSE; // Comando do IUP para fechar o MainLoop
}

int main(int argc, char **argv) {
    IupOpen(&argc, &argv);
    IupControlsOpen(); // <-- Essa função precisa do <iupcontrols.h>
    IupSetGlobal("UTF8MODE", "YES");
    IupSetGlobal("UTF8MODE_FILE", "YES");

    pers_inicializar(TIPO_PERSISTENCIA);

    Ihandle *menu = IupMenu(
        IupSubmenu("Cadastros", IupMenu(
            IupItem("Produtora", "btn_abrir_produtora_cb"),
            IupItem("Clientes", "btn_abrir_clientes_cb"),
            IupItem("Equipe Interna", "btn_abrir_equipe_cb"), 
            IupItem("Recursos/Equipamentos", "btn_abrir_recursos_cb"),
            IupItem("Fornecedores/Parceiros", "btn_abrir_fornecedores_cb"),
            IupItem("Operadores do Sistema", "btn_abrir_operadores_cb"),
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
    IupSetAttribute(dlg_main, "SIZE", "720x480");
    
    // Associa o menu à janela principal
    IupSetAttributeHandle(dlg_main, "MENU", menu);
    
    // Registra os callbacks (só precisa registrar 1x)
    IupSetFunction("btn_abrir_clientes_cb", (Icallback)btn_abrir_clientes_cb);
    IupSetFunction("btn_abrir_equipe_cb", (Icallback)btn_abrir_equipe_cb);
    IupSetFunction("btn_abrir_recursos_cb", (Icallback)btn_abrir_recursos_cb);
    IupSetFunction("btn_abrir_fornecedores_cb", (Icallback)btn_abrir_fornecedores_cb);
    IupSetFunction("btn_abrir_operadores_cb", (Icallback)btn_abrir_operadores_cb);
    IupSetFunction("btn_abrir_produtora_cb", (Icallback)btn_abrir_produtora_cb);
    IupSetFunction("btn_sair_cb", (Icallback)btn_sair_cb);

    IupShowXY(dlg_main, IUP_CENTER, IUP_CENTER);
    IupMainLoop();
    IupClose();
    return 0;
}