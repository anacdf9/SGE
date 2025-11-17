#include <stdlib.h>
#include <string.h>
#include "../model/pers.h"
#include "operador_controller.h"

static int operador_next_id(void){ Operador v[1024]; int n=operador_listar(v,1024); int m=0; for(int i=0;i<n;i++) if(v[i].id>m) m=v[i].id; return m+1; }

int operador_salvar(Operador o) { if (o.id <= 0) { o.id = operador_next_id(); if(o.id<=0) o.id=1; } return pers_salvar_operador(o); }
int operador_excluir(int id) { return pers_remover_operador(id); }
int operador_listar(Operador *buffer, int max) { return pers_carregar_operadores(buffer, max); }

int operador_salvar_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    Ihandle *txtNome = (Ihandle*)IupGetAttribute(self, "txtNome");
    Ihandle *txtUser = (Ihandle*)IupGetAttribute(self, "txtUser");
    Ihandle *txtSenha = (Ihandle*)IupGetAttribute(self, "txtSenha");

    Operador o;
    o.id = atoi(IupGetAttribute(txtId, "VALUE")); /* se <=0, será gerado em operador_salvar */
    strcpy(o.nome, IupGetAttribute(txtNome, "VALUE"));
    strcpy(o.usuario, IupGetAttribute(txtUser, "VALUE"));
    strcpy(o.senha, IupGetAttribute(txtSenha, "VALUE"));

    if (operador_salvar(o)) { IupSetfAttribute(txtId,"VALUE","%d", o.id); IupMessage("Sucesso", "Operador salvo."); } else IupMessage("Erro", "Falha ao salvar.");
    return IUP_DEFAULT;
}

int operador_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));
    if (id <= 0) { IupMessage("Erro", "ID inválido."); return IUP_DEFAULT; }
    if (operador_excluir(id)) IupMessage("Sucesso", "Operador excluído."); else IupMessage("Erro", "Falha ao excluir.");
    return IUP_DEFAULT;
}
