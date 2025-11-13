#include <iup.h>
#include <iupcontrols.h>
#include "../controller/cliente_controller.h" // Inclui os callbacks (_cb)
#include "cliente_view.h" // Inclui o próprio .h (boa prática)
#include <stdlib.h>   

// Guarda a janela do cliente pra não criar várias
static Ihandle* dlg_cliente = NULL;

/*
 * Função que cria a tela de cadastro de cliente
 * (Essa função é interna, só a 'mostrar' chama)
 */
Ihandle* cliente_view_create(void) {
    Ihandle *txtId = IupText(NULL);
    Ihandle *txtNome = IupText(NULL);
    Ihandle *txtEndereco = IupText(NULL);
    Ihandle *txtCpfCnpj = IupText(NULL);
    Ihandle *txtTelefone = IupText(NULL);
    Ihandle *txtEmail = IupText(NULL);
    Ihandle *txtContato = IupText(NULL);

    Ihandle *btnSalvar = IupButton("Salvar", NULL);
    Ihandle *btnExcluir = IupButton("Excluir", NULL);
    Ihandle *btnEditar = IupButton("Editar", NULL);

    // Monta o formulário
    Ihandle *form_vbox = IupVbox(
        IupLabel("Cadastro de Cliente"),
        IupGridBox(
            IupLabel("Código:"), txtId,
            IupLabel("Nome/Razão Social:"), txtNome,
            IupLabel("Endereço:"), txtEndereco,
            IupLabel("CPF/CNPJ:"), txtCpfCnpj,
            IupLabel("Telefone:"), txtTelefone,
            IupLabel("E-mail:"), txtEmail,
            IupLabel("Contato:"), txtContato,
            NULL
        ),
        IupHbox(btnSalvar, btnEditar, btnExcluir, NULL),
        NULL
    );
    IupSetAttribute(form_vbox, "MARGIN", "10x10");
    
    // Associa os handles dos campos de texto aos botões
    IupSetAttribute(btnSalvar, "txtId", (char*)txtId);
    IupSetAttribute(btnSalvar, "txtNome", (char*)txtNome);
    IupSetAttribute(btnSalvar, "txtEndereco", (char*)txtEndereco);
    IupSetAttribute(btnSalvar, "txtCpfCnpj", (char*)txtCpfCnpj);
    IupSetAttribute(btnSalvar, "txtTelefone", (char*)txtTelefone);
    IupSetAttribute(btnSalvar, "txtEmail", (char*)txtEmail);
    IupSetAttribute(btnSalvar, "txtContato", (char*)txtContato);

    IupSetAttribute(btnExcluir, "txtId", (char*)txtId);

    // Define os callbacks (usando os nomes com _cb)
    IupSetCallback(btnSalvar, "ACTION", (Icallback)cliente_salvar_cb);
    IupSetCallback(btnExcluir, "ACTION", (Icallback)cliente_excluir_cb);
    IupSetCallback(btnEditar, "ACTION", (Icallback)cliente_editar_cb);

    return form_vbox; // Retorna o Vbox (o formulário)
}


/*
 * FUNÇÃO QUE ESTAVA FALTANDO
 * É chamada pelo main.c para mostrar a tela
 */
void cliente_view_mostrar(void) {
    // Se a dialog ainda não foi criada, cria
    if (dlg_cliente == NULL) {
        
        // 1. Cria o formulário (o Vbox)
        Ihandle* form_cliente = cliente_view_create();
        
        // 2. Coloca o formulário dentro de uma Dialog (Janela)
        dlg_cliente = IupDialog(form_cliente);
        IupSetAttribute(dlg_cliente, "TITLE", "Cadastro de Clientes");
        IupSetAttribute(dlg_cliente, "SIZE", "300x250"); // Um tamanho
    }

    // Mostra a dialog (janela)
    IupShowXY(dlg_cliente, IUP_CENTER, IUP_CENTER);
}