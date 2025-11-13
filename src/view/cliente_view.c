#include <stdio.h>
#include "iup.h"
#include "iupcontrols.h"
#include "../controller/cliente_controller.h"

void cliente_view_mostrar()
{
    Ihandle *lblTitulo, *txtCodigo, *txtNome, *txtEndereco, *txtCpfCnpj,
            *txtTelefone, *txtEmail, *txtContato, *btnSalvar, *btnEditar,
            *btnExcluir, *btnSair, *form, *dlg;

    lblTitulo = IupLabel("Cadastro de Clientes");
    IupSetAttribute(lblTitulo, "ALIGNMENT", "ACENTER");
    IupSetAttribute(lblTitulo, "FONTSIZE", "18");
    IupSetAttribute(lblTitulo, "EXPAND", "HORIZONTAL");

    txtCodigo   = IupText(NULL);
    txtNome     = IupText(NULL);
    txtEndereco = IupText(NULL);
    txtCpfCnpj  = IupText(NULL);
    txtTelefone = IupText(NULL);
    txtEmail    = IupText(NULL);
    txtContato  = IupText(NULL);

    Ihandle *texts[] = { txtCodigo, txtNome, txtEndereco, txtCpfCnpj, txtTelefone, txtEmail, txtContato };
    for (int i = 0; i < 7; i++)
    {
        IupSetAttribute(texts[i], "SIZE", "300x20");
        IupSetAttribute(texts[i], "EXPAND", "HORIZONTAL");
    }

    btnSalvar = IupButton("Salvar", NULL);
    btnEditar = IupButton("Editar", NULL);
    btnExcluir = IupButton("Excluir", NULL);
    btnSair = IupButton("Sair", NULL);

    Ihandle *botoes = IupHbox(
        IupFill(),
        btnSalvar,
        IupFill(),
        btnEditar,
        IupFill(),
        btnExcluir,
        IupFill(),
        btnSair,
        IupFill(),
        NULL
    );
    IupSetAttribute(botoes, "ALIGNMENT", "ACENTER");
    IupSetAttribute(botoes, "GAP", "10");
    IupSetAttribute(botoes, "MARGIN", "10x10");

    form = IupVbox(
        lblTitulo,
        IupLabel("Código:"), txtCodigo,
        IupLabel("Nome / Razão Social:"), txtNome,
        IupLabel("Endereço completo:"), txtEndereco,
        IupLabel("CPF / CNPJ:"), txtCpfCnpj,
        IupLabel("Telefone:"), txtTelefone,
        IupLabel("E-mail:"), txtEmail,
        IupLabel("Nome do Contato:"), txtContato,
        botoes,
        NULL
    );
    IupSetAttribute(form, "ALIGNMENT", "ACENTER");
    IupSetAttribute(form, "GAP", "5");
    IupSetAttribute(form, "MARGIN", "20x20");

    dlg = IupDialog(form);
    IupSetAttribute(dlg, "TITLE", "SGE - Cadastro de Clientes");
    IupSetAttribute(dlg, "SIZE", "HALFxHALF");
    IupSetAttribute(dlg, "RESIZE", "YES");

    IupSetCallback(btnSalvar, "ACTION", (Icallback)cliente_controller_salvar);
    IupSetCallback(btnEditar, "ACTION", (Icallback)cliente_controller_editar);
    IupSetCallback(btnExcluir, "ACTION", (Icallback)cliente_controller_excluir);
    IupSetCallback(btnSair, "ACTION", (Icallback)IupExitLoop);

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
}
