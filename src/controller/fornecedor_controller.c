#include <stdlib.h>
#include <string.h>
#include "../model/pers.h"
#include "fornecedor_controller.h"

int fornecedor_salvar(Fornecedor f) { if (f.id <= 0) return 0; return pers_salvar_fornecedor(f); }
int fornecedor_excluir(int id) { return pers_remover_fornecedor(id); }
int fornecedor_listar(Fornecedor *buffer, int max) { return pers_carregar_fornecedores(buffer, max); }

int fornecedor_salvar_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    Ihandle *txtNF = (Ihandle*)IupGetAttribute(self, "txtNF");
    Ihandle *txtRS = (Ihandle*)IupGetAttribute(self, "txtRS");
    Ihandle *txtDoc = (Ihandle*)IupGetAttribute(self, "txtDoc");
    Ihandle *txtEnd = (Ihandle*)IupGetAttribute(self, "txtEnd");
    Ihandle *txtTel = (Ihandle*)IupGetAttribute(self, "txtTel");
    Ihandle *txtServ = (Ihandle*)IupGetAttribute(self, "txtServ");

    Fornecedor f;
    f.id = atoi(IupGetAttribute(txtId, "VALUE"));
    if (f.id <= 0) { IupMessage("Erro", "ID inválido."); return IUP_DEFAULT; }
    strcpy(f.nome_fantasia, IupGetAttribute(txtNF, "VALUE"));
    strcpy(f.razao_social, IupGetAttribute(txtRS, "VALUE"));
    strcpy(f.cnpj_cpf, IupGetAttribute(txtDoc, "VALUE"));
    strcpy(f.endereco, IupGetAttribute(txtEnd, "VALUE"));
    strcpy(f.telefone, IupGetAttribute(txtTel, "VALUE"));
    strcpy(f.tipo_servico, IupGetAttribute(txtServ, "VALUE"));

    if (fornecedor_salvar(f)) IupMessage("Sucesso", "Fornecedor salvo."); else IupMessage("Erro", "Falha ao salvar.");
    return IUP_DEFAULT;
}

int fornecedor_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));
    if (id <= 0) { IupMessage("Erro", "ID inválido."); return IUP_DEFAULT; }
    if (fornecedor_excluir(id)) IupMessage("Sucesso", "Fornecedor excluído."); else IupMessage("Erro", "Falha ao excluir.");
    return IUP_DEFAULT;
}
