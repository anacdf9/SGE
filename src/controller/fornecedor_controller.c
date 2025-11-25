/*
===============================================================================
   FORNECEDOR CONTROLLER
   
   Responsável por:
   - Gerenciar cadastro de fornecedores e parceiros
   - Controlar tipos de serviços prestados
   - Callbacks para interface gráfica
===============================================================================
*/

#include <stdlib.h>
#include <string.h>
#include "../model/pers.h"
#include "fornecedor_controller.h"


/* ========================================
   FUNÇÕES AUXILIARES
   ======================================== */

// Gera o próximo ID disponível para fornecedor
static int fornecedor_next_id(void) {
    Fornecedor v[1024];
    int n = fornecedor_listar(v, 1024);
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

// Salva ou atualiza um fornecedor
int fornecedor_salvar(Fornecedor f) {
    if (f.id <= 0) {
        f.id = fornecedor_next_id();
        if (f.id <= 0) f.id = 1;
    }
    return pers_salvar_fornecedor(f);
}

// Exclui um fornecedor
int fornecedor_excluir(int id) {
    return pers_remover_fornecedor(id);
}

// Lista todos os fornecedores
int fornecedor_listar(Fornecedor *buffer, int max) {
    return pers_carregar_fornecedores(buffer, max);
}

// Busca um fornecedor específico pelo ID
int fornecedor_obter(int id, Fornecedor *out) {
    if (!out || id <= 0) return 0;
    
    Fornecedor v[1024];
    int n = fornecedor_listar(v, 1024);
    
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

// Callback do botão Salvar
int fornecedor_salvar_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    Ihandle *txtNF = (Ihandle*)IupGetAttribute(self, "txtNF");
    Ihandle *txtRS = (Ihandle*)IupGetAttribute(self, "txtRS");
    Ihandle *txtDoc = (Ihandle*)IupGetAttribute(self, "txtDoc");
    Ihandle *txtEnd = (Ihandle*)IupGetAttribute(self, "txtEnd");
    Ihandle *txtTel = (Ihandle*)IupGetAttribute(self, "txtTel");
    Ihandle *txtServ = (Ihandle*)IupGetAttribute(self, "txtServ");

    Fornecedor f;
    f.id = atoi(IupGetAttribute(txtId, "VALUE")); /* se <=0, será gerado em fornecedor_salvar */
    strcpy(f.nome_fantasia, IupGetAttribute(txtNF, "VALUE"));
    strcpy(f.razao_social, IupGetAttribute(txtRS, "VALUE"));
    strcpy(f.cnpj_cpf, IupGetAttribute(txtDoc, "VALUE"));
    strcpy(f.endereco, IupGetAttribute(txtEnd, "VALUE"));
    strcpy(f.telefone, IupGetAttribute(txtTel, "VALUE"));
    strcpy(f.tipo_servico, IupGetAttribute(txtServ, "VALUE"));

    if (fornecedor_salvar(f)) {
        IupSetfAttribute(txtId, "VALUE", "%d", f.id);
        IupMessage("Sucesso", "Fornecedor salvo.");
    } else {
        IupMessage("Erro", "Falha ao salvar.");
    }
    
    return IUP_DEFAULT;
}

// Callback do botão Excluir
int fornecedor_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));
    
    if (id <= 0) {
        IupMessage("Erro", "ID inválido.");
        return IUP_DEFAULT;
    }
    
    if (fornecedor_excluir(id)) {
        IupMessage("Sucesso", "Fornecedor excluído.");
    } else {
        IupMessage("Erro", "Falha ao excluir.");
    }
    
    return IUP_DEFAULT;
}
