/*
===============================================================================
   EQUIPE CONTROLLER
   
   Responsável por:
   - Gerenciar cadastro de membros da equipe interna
   - Controlar valores por hora de cada membro
   - Callbacks para interface gráfica
===============================================================================
*/

#include <stdlib.h>
#include <string.h>
#include "../model/pers.h"
#include "equipe_controller.h"
#include <iup.h>


/* ========================================
   FUNÇÕES AUXILIARES
   ======================================== */

// Gera o próximo ID disponível para equipe
static int equipe_next_id(void) {
    Equipe v[1024];
    int n = pers_carregar_equipes(v, 1024);
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

// Salva ou atualiza um membro da equipe
int equipe_salvar(Equipe e) {
    if (e.id <= 0) {
        e.id = equipe_next_id();
        if (e.id <= 0) e.id = 1;
    }
    return pers_salvar_equipe(e);
}

// Exclui um membro da equipe
int equipe_excluir(int id) {
    return pers_remover_equipe(id);
}

// Lista todos os membros da equipe
int equipe_listar(Equipe *buffer, int max) {
    return pers_carregar_equipes(buffer, max);
}

// Busca um membro da equipe específico pelo ID
int equipe_obter(int id, Equipe *out) {
    if (!out || id <= 0) return 0;
    
    Equipe v[1024];
    int n = equipe_listar(v, 1024);
    
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
int equipe_salvar_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    Ihandle *txtNome = (Ihandle*)IupGetAttribute(self, "txtNome");
    Ihandle *txtCpf = (Ihandle*)IupGetAttribute(self, "txtCpf");
    Ihandle *txtFunc = (Ihandle*)IupGetAttribute(self, "txtFunc");
    Ihandle *txtValor = (Ihandle*)IupGetAttribute(self, "txtValor");

    Equipe e;
    e.id = atoi(IupGetAttribute(txtId, "VALUE")); /* se <=0, será gerado em equipe_salvar */
    strcpy(e.nome, IupGetAttribute(txtNome, "VALUE"));
    strcpy(e.cpf, IupGetAttribute(txtCpf, "VALUE"));
    strcpy(e.funcao, IupGetAttribute(txtFunc, "VALUE"));
    e.valor_hora = atof(IupGetAttribute(txtValor, "VALUE"));

    if (equipe_salvar(e)) {
        IupSetfAttribute(txtId, "VALUE", "%d", e.id);
        IupMessage("Sucesso", "Equipe salva.");
    } else {
        IupMessage("Erro", "Falha ao salvar.");
    }
    
    return IUP_DEFAULT;
}

// Callback do botão Excluir
int equipe_excluir_cb(Ihandle *self) {
    Ihandle *txtId = (Ihandle*)IupGetAttribute(self, "txtId");
    int id = atoi(IupGetAttribute(txtId, "VALUE"));
    
    if (id <= 0) {
        IupMessage("Erro", "ID inválido.");
        return IUP_DEFAULT;
    }
    
    if (equipe_excluir(id)) {
        IupMessage("Sucesso", "Registro excluído.");
    } else {
        IupMessage("Erro", "Falha ao excluir.");
    }
    
    return IUP_DEFAULT;
}
