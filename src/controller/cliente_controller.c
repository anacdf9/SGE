#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../model/cliente.h"
#include "../model/pers.h"
#include "cliente_controller.h"

/* Salva/atualiza cliente */
int cliente_salvar(Cliente c) {
    return pers_salvar_cliente(c);
}

/* Remove cliente */
int cliente_excluir(int id) {
    return pers_remover_cliente(id);
}

/* Lista clientes */
int cliente_listar(Cliente *buffer, int max) {
    return pers_carregar_clientes(buffer, max);
}

/* === CALLBACKS (usadas pelo IUP) === */

/* Callback do botão Salvar */
int cliente_salvar_cb(Ihandle *self) {
    Ihandle *txtNome = (Ihandle*)IupGetAttribute(self, "txtNome");
    Ihandle *txtIdade = (Ihandle*)IupGetAttribute(self, "txtIdade");

    Cliente c;
    strcpy(c.nome, IupGetAttribute(txtNome, "VALUE"));
    c.idade = atoi(IupGetAttribute(txtIdade, "VALUE"));

    if (cliente_salvar(c))
        IupMessage("Sucesso", "Cliente salvo com sucesso!");
    else
        IupMessage("Erro", "Falha ao salvar cliente.");

    return IUP_DEFAULT;
}

/* Callback do botão Excluir */
int cliente_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));

    if (cliente_excluir(id))
        IupMessage("Sucesso", "Cliente excluído com sucesso!");
    else
        IupMessage("Erro", "Falha ao excluir cliente.");

    return IUP_DEFAULT;
}

/* Callback do botão Editar */
int cliente_editar_cb(Ihandle *self) {
    IupMessage("Editar", "Função de edição ainda não implementada.");
    return IUP_DEFAULT;
}
