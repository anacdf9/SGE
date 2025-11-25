/*
===============================================================================
   PRODUTORA CONTROLLER
   
   Responsável por:
   - Gerenciar dados da produtora (empresa)
   - Controlar margem de lucro padrão
   - Callbacks para interface gráfica
   
   IMPORTANTE: A margem de lucro é usada no cálculo de preço de locação!
===============================================================================
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../model/pers.h"
#include "produtora_controller.h"


/* ========================================
   FUNÇÕES PRINCIPAIS
   ======================================== */

// Salva ou atualiza dados da produtora
int produtora_salvar(Produtora p) {
    return pers_salvar_produtora(p);
}

// Obtém os dados da produtora
int produtora_obter(Produtora *out) {
    return pers_obter_produtora(out);
}

// Remove os dados da produtora
int produtora_remover(void) {
    return pers_remover_produtora();
}


/* ========================================
   CALLBACKS PARA INTERFACE GRÁFICA (IUP)
   ======================================== */

// Callback do botão Salvar
int produtora_salvar_cb(Ihandle *self) {
    Ihandle *tNF = (Ihandle*)IupGetAttribute(self, "tNF");
    Ihandle *tRS = (Ihandle*)IupGetAttribute(self, "tRS");
    Ihandle *tIE = (Ihandle*)IupGetAttribute(self, "tIE");
    Ihandle *tCNPJ = (Ihandle*)IupGetAttribute(self, "tCNPJ");
    Ihandle *tEnd = (Ihandle*)IupGetAttribute(self, "tEnd");
    Ihandle *tTel = (Ihandle*)IupGetAttribute(self, "tTel");
    Ihandle *tEmail = (Ihandle*)IupGetAttribute(self, "tEmail");
    Ihandle *tResp = (Ihandle*)IupGetAttribute(self, "tResp");
    Ihandle *tTelResp = (Ihandle*)IupGetAttribute(self, "tTelResp");
    Ihandle *tMargem = (Ihandle*)IupGetAttribute(self, "tMargem");

    Produtora p; memset(&p, 0, sizeof(p));
    strcpy(p.nome_fantasia, IupGetAttribute(tNF, "VALUE"));
    strcpy(p.razao_social, IupGetAttribute(tRS, "VALUE"));
    strcpy(p.inscricao_estadual, IupGetAttribute(tIE, "VALUE"));
    strcpy(p.cnpj, IupGetAttribute(tCNPJ, "VALUE"));
    strcpy(p.endereco, IupGetAttribute(tEnd, "VALUE"));
    strcpy(p.telefone, IupGetAttribute(tTel, "VALUE"));
    strcpy(p.email, IupGetAttribute(tEmail, "VALUE"));
    strcpy(p.responsavel, IupGetAttribute(tResp, "VALUE"));
    strcpy(p.telefone_responsavel, IupGetAttribute(tTelResp, "VALUE"));
    p.margem_lucro_padrao = atof(IupGetAttribute(tMargem, "VALUE"));

    if (produtora_salvar(p)) IupMessage("Sucesso", "Dados da Produtora salvos."); else IupMessage("Erro", "Falha ao salvar.");
    return IUP_DEFAULT;
}

int produtora_carregar_cb(Ihandle *self) {
    Ihandle *tNF = (Ihandle*)IupGetAttribute(self, "tNF");
    Ihandle *tRS = (Ihandle*)IupGetAttribute(self, "tRS");
    Ihandle *tIE = (Ihandle*)IupGetAttribute(self, "tIE");
    Ihandle *tCNPJ = (Ihandle*)IupGetAttribute(self, "tCNPJ");
    Ihandle *tEnd = (Ihandle*)IupGetAttribute(self, "tEnd");
    Ihandle *tTel = (Ihandle*)IupGetAttribute(self, "tTel");
    Ihandle *tEmail = (Ihandle*)IupGetAttribute(self, "tEmail");
    Ihandle *tResp = (Ihandle*)IupGetAttribute(self, "tResp");
    Ihandle *tTelResp = (Ihandle*)IupGetAttribute(self, "tTelResp");
    Ihandle *tMargem = (Ihandle*)IupGetAttribute(self, "tMargem");

    Produtora p;
    if (!produtora_obter(&p)) { IupMessage("Aviso", "Nenhum dado salvo ainda."); return IUP_DEFAULT; }

    IupSetStrAttribute(tNF, "VALUE", p.nome_fantasia);
    IupSetStrAttribute(tRS, "VALUE", p.razao_social);
    IupSetStrAttribute(tIE, "VALUE", p.inscricao_estadual);
    IupSetStrAttribute(tCNPJ, "VALUE", p.cnpj);
    IupSetStrAttribute(tEnd, "VALUE", p.endereco);
    IupSetStrAttribute(tTel, "VALUE", p.telefone);
    IupSetStrAttribute(tEmail, "VALUE", p.email);
    IupSetStrAttribute(tResp, "VALUE", p.responsavel);
    IupSetStrAttribute(tTelResp, "VALUE", p.telefone_responsavel);
    char buf[64];
    sprintf(buf, "%.2f", p.margem_lucro_padrao);
    IupSetStrAttribute(tMargem, "VALUE", buf);
    IupMessage("Sucesso", "Dados da Produtora carregados.");
    return IUP_DEFAULT;
}

int produtora_excluir_cb(Ihandle *self) {
    Ihandle *tNF = (Ihandle*)IupGetAttribute(self, "tNF");
    Ihandle *tRS = (Ihandle*)IupGetAttribute(self, "tRS");
    Ihandle *tIE = (Ihandle*)IupGetAttribute(self, "tIE");
    Ihandle *tCNPJ = (Ihandle*)IupGetAttribute(self, "tCNPJ");
    Ihandle *tEnd = (Ihandle*)IupGetAttribute(self, "tEnd");
    Ihandle *tTel = (Ihandle*)IupGetAttribute(self, "tTel");
    Ihandle *tEmail = (Ihandle*)IupGetAttribute(self, "tEmail");
    Ihandle *tResp = (Ihandle*)IupGetAttribute(self, "tResp");
    Ihandle *tTelResp = (Ihandle*)IupGetAttribute(self, "tTelResp");
    Ihandle *tMargem = (Ihandle*)IupGetAttribute(self, "tMargem");

    if (produtora_remover()) {
        IupSetAttribute(tNF, "VALUE", "");
        IupSetAttribute(tRS, "VALUE", "");
        IupSetAttribute(tIE, "VALUE", "");
        IupSetAttribute(tCNPJ, "VALUE", "");
        IupSetAttribute(tEnd, "VALUE", "");
        IupSetAttribute(tTel, "VALUE", "");
        IupSetAttribute(tEmail, "VALUE", "");
        IupSetAttribute(tResp, "VALUE", "");
        IupSetAttribute(tTelResp, "VALUE", "");
        IupSetAttribute(tMargem, "VALUE", "");
        IupMessage("Sucesso", "Dados da Produtora excluídos.");
    } else {
        IupMessage("Aviso", "Nenhum dado para excluir.");
    }
    return IUP_DEFAULT;
}
