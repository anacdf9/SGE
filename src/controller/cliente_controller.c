#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../model/cliente.h"
#include "../model/pers.h"
#include "cliente_controller.h"

/* Salva/atualiza cliente */
int cliente_salvar(Cliente c) {
    // Validação básica (ID não pode ser 0 ou negativo)
    if (c.id <= 0) {
        return 0; // Falha
    }
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
    
    // Pega os 'handles' (ponteiros) dos campos de texto que a view associou
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    Ihandle *txtNome = (Ihandle*)IupGetAttribute(self, "txtNome");
    Ihandle *txtEndereco = (Ihandle*)IupGetAttribute(self, "txtEndereco");
    Ihandle *txtCpfCnpj = (Ihandle*)IupGetAttribute(self, "txtCpfCnpj");
    Ihandle *txtTelefone = (Ihandle*)IupGetAttribute(self, "txtTelefone");
    Ihandle *txtEmail = (Ihandle*)IupGetAttribute(self, "txtEmail");
    Ihandle *txtContato = (Ihandle*)IupGetAttribute(self, "txtContato");

    Cliente c; // Cria uma struct temporária

    // 1. Pega o ID (Código)
    int id_cliente = atoi(IupGetAttribute(txtId, "VALUE"));
    if (id_cliente <= 0) {
        IupMessage("Erro", "O Código (ID) do cliente é inválido ou não foi preenchido.");
        return IUP_DEFAULT;
    }
    c.id = id_cliente;

    // 2. Pega os demais campos do formulário
    strcpy(c.nome, IupGetAttribute(txtNome, "VALUE"));
    strcpy(c.endereco, IupGetAttribute(txtEndereco, "VALUE"));
    strcpy(c.cpf_cnpj, IupGetAttribute(txtCpfCnpj, "VALUE"));
    strcpy(c.telefone, IupGetAttribute(txtTelefone, "VALUE"));
    strcpy(c.email, IupGetAttribute(txtEmail, "VALUE"));
    strcpy(c.contato, IupGetAttribute(txtContato, "VALUE"));

    // 3. Manda salvar
    if (cliente_salvar(c))
        IupMessage("Sucesso", "Cliente salvo com sucesso!");
    else
        IupMessage("Erro", "Falha ao salvar cliente. (Verifique se o Código é válido).");

    return IUP_DEFAULT;
}

/* Callback do botão Excluir */
int cliente_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));

    if (id <= 0) {
         IupMessage("Erro", "ID inválido para exclusão.");
         return IUP_DEFAULT;
    }

    if (cliente_excluir(id))
        IupMessage("Sucesso", "Cliente excluído com sucesso!");
    else
        IupMessage("Erro", "Falha ao excluir cliente (ID não encontrado?).");

    return IUP_DEFAULT;
}

/* Callback do botão Editar */
int cliente_editar_cb(Ihandle *self) {
    IupMessage("Editar", "Função de edição ainda não implementada.");
    return IUP_DEFAULT;
}