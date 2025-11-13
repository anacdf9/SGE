#include <stdlib.h>
#include <string.h>
#include "../model/pers.h"
#include "recurso_controller.h"

int recurso_salvar(Recurso r) {
    if (r.id <= 0) return 0;
    return pers_salvar_recurso(r);
}

int recurso_excluir(int id) {
    return pers_remover_recurso(id);
}

int recurso_listar(Recurso *buffer, int max) {
    return pers_carregar_recursos(buffer, max);
}

int recurso_salvar_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    Ihandle *txtDesc = (Ihandle*)IupGetAttribute(self, "txtDesc");
    Ihandle *txtCat = (Ihandle*)IupGetAttribute(self, "txtCat");
    Ihandle *txtQtd = (Ihandle*)IupGetAttribute(self, "txtQtd");
    Ihandle *txtCusto = (Ihandle*)IupGetAttribute(self, "txtCusto");
    Ihandle *txtLoc = (Ihandle*)IupGetAttribute(self, "txtLoc");

    Recurso r;
    r.id = atoi(IupGetAttribute(txtId, "VALUE"));
    if (r.id <= 0) { IupMessage("Erro", "ID inválido."); return IUP_DEFAULT; }
    strcpy(r.descricao, IupGetAttribute(txtDesc, "VALUE"));
    strcpy(r.categoria, IupGetAttribute(txtCat, "VALUE"));
    r.quantidade = atoi(IupGetAttribute(txtQtd, "VALUE"));
    r.preco_custo = atof(IupGetAttribute(txtCusto, "VALUE"));
    r.valor_locacao = atof(IupGetAttribute(txtLoc, "VALUE"));

    if (recurso_salvar(r)) IupMessage("Sucesso", "Recurso salvo."); else IupMessage("Erro", "Falha ao salvar.");
    return IUP_DEFAULT;
}

int recurso_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));
    if (id <= 0) { IupMessage("Erro", "ID inválido."); return IUP_DEFAULT; }
    if (recurso_excluir(id)) IupMessage("Sucesso", "Recurso excluído."); else IupMessage("Erro", "Falha ao excluir.");
    return IUP_DEFAULT;
}
