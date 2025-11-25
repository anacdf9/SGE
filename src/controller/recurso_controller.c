/*
===============================================================================
   RECURSO CONTROLLER
   
   Responsável por:
   - Gerenciar cadastro de recursos/equipamentos
   - Controlar estoque de recursos
   - Atualizar preços de locação
   - Callbacks para interface gráfica
===============================================================================
*/

#include <stdlib.h>
#include <string.h>
#include "../model/pers.h"
#include "recurso_controller.h"
#include <iup.h>


/* ========================================
   FUNÇÕES AUXILIARES
   ======================================== */

// Gera o próximo ID disponível para recurso
static int recurso_next_id(void) {
    Recurso v[1024];
    int n = recurso_listar(v, 1024);
    int m = 0;
    
    for (int i = 0; i < n; i++) {
        if (v[i].id > m) {
            m = v[i].id;
        }
    }
    
    return m + 1;
}


/* ========================================
   FUNÇÕES PRINCIPAIS
   ======================================== */

// Salva ou atualiza um recurso
int recurso_salvar(Recurso r) {
    // Gera ID se necessário
    if (r.id <= 0) {
        r.id = recurso_next_id();
        if (r.id <= 0) r.id = 1;
    }
    
    return pers_salvar_recurso(r);
}

// Exclui um recurso do cadastro
int recurso_excluir(int id) {
    return pers_remover_recurso(id);
}

// Lista todos os recursos cadastrados
int recurso_listar(Recurso *buffer, int max) {
    return pers_carregar_recursos(buffer, max);
}

// Busca um recurso específico pelo ID
int recurso_obter(int id, Recurso *out) {
    if (!out || id <= 0) return 0;
    
    Recurso v[1024];
    int n = recurso_listar(v, 1024);
    
    for (int i = 0; i < n; i++) {
        if (v[i].id == id) {
            *out = v[i];
            return 1;
        }
    }
    
    return 0;
}


/* ========================================
   CALLBACKS PARA INTERFACE GRÁFICA (IUP)
   ======================================== */

// Callback do botão Salvar - captura dados do formulário
int recurso_salvar_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    Ihandle *txtDesc = (Ihandle*)IupGetAttribute(self, "txtDesc");
    Ihandle *txtCat = (Ihandle*)IupGetAttribute(self, "txtCat");
    Ihandle *txtQtd = (Ihandle*)IupGetAttribute(self, "txtQtd");
    Ihandle *txtCusto = (Ihandle*)IupGetAttribute(self, "txtCusto");
    Ihandle *txtLoc = (Ihandle*)IupGetAttribute(self, "txtLoc");

    Recurso r;
    r.id = atoi(IupGetAttribute(txtId, "VALUE")); /* se <=0, será gerado em recurso_salvar */
    strcpy(r.descricao, IupGetAttribute(txtDesc, "VALUE"));
    strcpy(r.categoria, IupGetAttribute(txtCat, "VALUE"));
    r.quantidade = atoi(IupGetAttribute(txtQtd, "VALUE"));
    r.preco_custo = atof(IupGetAttribute(txtCusto, "VALUE"));
    r.valor_locacao = atof(IupGetAttribute(txtLoc, "VALUE"));

    if (recurso_salvar(r)) { IupSetfAttribute(txtId,"VALUE","%d", r.id); IupMessage("Sucesso", "Recurso salvo."); } else IupMessage("Erro", "Falha ao salvar.");
    return IUP_DEFAULT;
}

int recurso_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));
    if (id <= 0) { IupMessage("Erro", "ID inválido."); return IUP_DEFAULT; }
    if (recurso_excluir(id)) IupMessage("Sucesso", "Recurso excluído."); else IupMessage("Erro", "Falha ao excluir.");
    return IUP_DEFAULT;
}
