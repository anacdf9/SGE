#include <stdlib.h>
#include <string.h>
#include "../model/pers.h"
#include "operador_controller.h"

int operador_salvar(Operador o) { if (o.id <= 0) return 0; return pers_salvar_operador(o); }
int operador_excluir(int id) { return pers_remover_operador(id); }
int operador_listar(Operador *buffer, int max) { return pers_carregar_operadores(buffer, max); }

int operador_salvar_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    Ihandle *txtNome = (Ihandle*)IupGetAttribute(self, "txtNome");
    Ihandle *txtUser = (Ihandle*)IupGetAttribute(self, "txtUser");
    Ihandle *txtSenha = (Ihandle*)IupGetAttribute(self, "txtSenha");

    Operador o;
    o.id = atoi(IupGetAttribute(txtId, "VALUE"));
    if (o.id <= 0) { IupMessage("Erro", "ID inválido."); return IUP_DEFAULT; }
    strcpy(o.nome, IupGetAttribute(txtNome, "VALUE"));
    strcpy(o.usuario, IupGetAttribute(txtUser, "VALUE"));
    strcpy(o.senha, IupGetAttribute(txtSenha, "VALUE"));

    if (operador_salvar(o)) IupMessage("Sucesso", "Operador salvo."); else IupMessage("Erro", "Falha ao salvar.");
    return IUP_DEFAULT;
}

int operador_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));
    if (id <= 0) { IupMessage("Erro", "ID inválido."); return IUP_DEFAULT; }
    if (operador_excluir(id)) IupMessage("Sucesso", "Operador excluído."); else IupMessage("Erro", "Falha ao excluir.");
    return IUP_DEFAULT;
}
