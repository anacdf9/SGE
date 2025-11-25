/*
 * ========================================================================
 * MÓDULO: Transações Financeiras
 * Descrição: Interface para gerenciar caixa, contas a receber,
 *            compras e contas a pagar
 * ========================================================================
 */

#include <iup.h>
#include <iupcontrols.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "../controller/transacoes_controller.h"
#include "../controller/recurso_controller.h"
#include "../controller/fornecedor_controller.h"
#include "../controller/evento_controller.h"
#include "../controller/produtora_controller.h"

#include "../model/transacoes.h"
#include "../model/recurso.h"
#include "../model/fornecedor.h"
#include "../model/evento.h"
#include "../model/produtora.h"

#include "ui_common.h"

static Ihandle *dlg = NULL;
/* Abas */
static Ihandle *mat_caixa, *lbl_saldo;
static Ihandle *mat_receber;
static Ihandle *mat_compras, *mat_compra_itens;
static Ihandle *mat_pagar;

/* Campos Caixa */
static Ihandle *txt_cx_data, *txt_cx_valor, *txt_cx_desc, *drop_cx_tipo, *drop_cx_forma;
/* Campos Receber */
static Ihandle *drop_cr_evento, *txt_cr_valor, *txt_cr_emissao, *txt_cr_venc;
static Ihandle *txt_cr_baixa_id, *txt_cr_baixa_valor, *drop_cr_baixa_forma;
/* Campos Compras */
static Ihandle *drop_cp_forn, *txt_cp_data, *txt_cp_frete, *txt_cp_imposto, *drop_cp_pag_tipo, *txt_cp_parcelas;
static Ihandle *drop_cp_item_recurso, *txt_cp_item_preco, *txt_cp_item_qtd;
/* Campos Pagar */
static Ihandle *txt_pg_id, *chk_pg_ocultar;

/* ========================================================================
 * FUNÇÕES AUXILIARES
 * ======================================================================== */

/*
 * Função: limpar_matriz
 * Descrição: Limpa todo o conteúdo de uma matriz (tabela)
 * Parâmetros:
 *   - mat: ponteiro para a matriz a ser limpa
 */
static void limpar_matriz(Ihandle *mat) {
    if (!mat) return;
    
    IupSetAttribute(mat, "NUMLIN", "1");
    int cols = IupGetInt(mat, "NUMCOL");
    
    for (int c = 1; c <= cols; c++) {
        IupSetStrAttributeId2(mat, "", 1, c, "");
    }
}

/* ========================================================================
 * FUNÇÕES DE RECARREGAMENTO DE DADOS
 * ======================================================================== */

/*
 * Função: reload_caixa
 * Descrição: Recarrega os dados da aba Caixa
 *            Atualiza o saldo e a lista de lançamentos
 */
static void reload_caixa(void) {
    /* Buscar lançamentos do caixa */
    CaixaLancamento arr[1024];
    int n = caixa_listar(arr, 1024);
    
    /* Definir número de linhas na matriz */
    IupSetInt(mat_caixa, "NUMLIN", n > 0 ? n : 1);
    
    /* Se não há lançamentos, limpa a matriz */
    if (n == 0) {
        limpar_matriz(mat_caixa);
        IupSetAttribute(mat_caixa, "REDRAW", "ALL");
        return;
    }
    
    /* Atualizar saldo */
    double saldo = caixa_saldo_atual();
    char buf[64];
    sprintf(buf, "Saldo: R$ %.2f", saldo);
    IupSetAttribute(lbl_saldo, "TITLE", buf);
    
    /* Preencher dados na matriz */
    for (int i = 0; i < n; i++) {
        int lin = i + 1;
        IupSetIntId2(mat_caixa, "", lin, 1, arr[i].id);
        IupSetStrAttributeId2(mat_caixa, "", lin, 2, arr[i].data);
        IupSetStrAttributeId2(mat_caixa, "", lin, 3, arr[i].tipo);
        IupSetStrAttributeId2(mat_caixa, "", lin, 4, arr[i].forma);
        IupSetfAttributeId2(mat_caixa, "", lin, 5, "R$ %.2f", arr[i].valor);
        IupSetStrAttributeId2(mat_caixa, "", lin, 6, arr[i].descricao);
    }
    
    /* Redesenhar matriz */
    IupSetAttribute(mat_caixa, "REDRAW", "ALL");
}

/*
 * Função: reload_receber
 * Descrição: Recarrega os dados da aba Contas a Receber
 */
static void reload_receber(void) {
    /* Buscar contas a receber */
    ContaReceber arr[1024];
    int n = conta_receber_listar(arr, 1024);
    
    IupSetInt(mat_receber, "NUMLIN", n > 0 ? n : 1);
    
    if (n == 0) {
        limpar_matriz(mat_receber);
        IupSetAttribute(mat_receber, "REDRAW", "ALL");
        return;
    }
    
    /* Preencher dados na matriz */
    for (int i = 0; i < n; i++) {
        int lin = i + 1;
        
        IupSetIntId2(mat_receber, "", lin, 1, arr[i].id);
        IupSetIntId2(mat_receber, "", lin, 2, arr[i].evento_id);
        IupSetfAttributeId2(mat_receber, "", lin, 3, "%.2f", arr[i].valor_fatura);
        IupSetfAttributeId2(mat_receber, "", lin, 4, "%.2f", arr[i].valor_recebido);
        
        /* Determinar status da conta */
        const char *st = arr[i].status == 0 ? "Aberto" : 
                        (arr[i].status == 1 ? "Parcial" : "Liquidado");
        IupSetStrAttributeId2(mat_receber, "", lin, 5, st);
        IupSetStrAttributeId2(mat_receber, "", lin, 6, arr[i].data_vencimento);
    }
    
    IupSetAttribute(mat_receber, "REDRAW", "ALL");
}

static void reload_list_eventos(void){
    if(!drop_cr_evento) return;
    Evento evs[512]; int n=evento_listar(evs,512);
    for(int i=1;i<600;i++){ IupSetAttributeId(drop_cr_evento,"", i, NULL); }
    if(n<=0){ IupSetAttribute(drop_cr_evento,"1","(Nenhum)"); return; }
    for(int i=0;i<n;i++){
        char buf[256]; snprintf(buf,sizeof(buf),"%d:%s", evs[i].id, evs[i].nome);
        IupSetAttributeId(drop_cr_evento,"", i+1, buf);
    }
}

static void reload_compras(void){
    Compra arr[512]; int n=compra_listar(arr,512); IupSetInt(mat_compras,"NUMLIN", n>0?n:1); if(n==0){ limpar_matriz(mat_compras); IupSetAttribute(mat_compras,"REDRAW","ALL"); return; }
    Fornecedor forn[256]; int nf = fornecedor_listar(forn,256);
    for(int i=0;i<n;i++){ int lin=i+1; IupSetIntId2(mat_compras,"",lin,1, arr[i].id); char fdesc[128]=""; for(int j=0;j<nf;j++){ if(forn[j].id==arr[i].fornecedor_id){ strncpy(fdesc, forn[j].nome_fantasia,127); break; } }
        IupSetStrAttributeId2(mat_compras,"",lin,2, fdesc);
        IupSetStrAttributeId2(mat_compras,"",lin,3, arr[i].data);
        IupSetfAttributeId2(mat_compras,"",lin,4,"%.2f", arr[i].total_nota);
        CompraItem itens[256]; int ni=compra_itens_por_compra(arr[i].id, itens, 256); double soma=0; int cont=0; for(int k=0;k<ni;k++){ if(itens[k].preco_locacao_unit>0){ soma+=itens[k].preco_locacao_unit; cont++; } }
        double media = (cont>0)? soma/cont : 0.0;
        IupSetfAttributeId2(mat_compras,"",lin,5,"%.2f", media);
        IupSetStrAttributeId2(mat_compras,"",lin,6, arr[i].pagamento_tipo==0?"A vista":"Prazo"); }
    IupSetAttribute(mat_compras,"REDRAW","ALL");
}

static void carregar_itens_compra(int compra_id){
    if(!mat_compra_itens) return;
    CompraItem itens[512]; int n = compra_itens_por_compra(compra_id, itens, 512);
    if(n<=0){ IupSetAttribute(mat_compra_itens, "NUMLIN", "1"); for(int c=1;c<=4;c++) IupSetStrAttributeId2(mat_compra_itens,"",1,c,""); IupSetAttribute(mat_compra_itens,"REDRAW","ALL"); return; }
    IupSetInt(mat_compra_itens, "NUMLIN", n);
    for(int i=0;i<n;i++){ int lin=i+1; IupSetIntId2(mat_compra_itens,"",lin,1,itens[i].recurso_id); IupSetfAttributeId2(mat_compra_itens,"",lin,2,"%.2f", itens[i].preco_custo_unit); IupSetIntId2(mat_compra_itens,"",lin,3,itens[i].quantidade); IupSetfAttributeId2(mat_compra_itens,"",lin,4,"%.2f", itens[i].preco_locacao_unit); }
    IupSetAttribute(mat_compra_itens,"REDRAW","ALL");
}

static int mat_compras_click_cb(Ihandle *self, int lin, int col, char *status){
    (void)self; (void)col; (void)status;
    if(lin<=0) return IUP_DEFAULT;
    const char *cid = IupGetAttributeId2(mat_compras, "", lin, 1);
    if(!cid) return IUP_DEFAULT;
    int compra_id = atoi(cid);
    if(compra_id>0) carregar_itens_compra(compra_id);
    return IUP_DEFAULT;
}

static void reload_list_fornecedores(void){
    if(!drop_cp_forn) return;
    Fornecedor forn[256]; int nf=fornecedor_listar(forn,256);
    for(int i=1;i<300;i++){ IupSetAttributeId(drop_cp_forn,"", i, NULL); }
    if(nf<=0){ IupSetAttribute(drop_cp_forn,"1","(Nenhum)"); return; }
    for(int i=0;i<nf;i++){
        char buf[256]; snprintf(buf,sizeof(buf),"%d:%s", forn[i].id, forn[i].nome_fantasia);
        IupSetAttributeId(drop_cp_forn,"", i+1, buf);
    }
}

static void reload_list_recursos(void){
    if(!drop_cp_item_recurso) return;
    for(int i=1;i<800;i++){ IupSetAttributeId(drop_cp_item_recurso, "", i, NULL); }
    Recurso recs[512]; int nr = recurso_listar(recs,512);
    if(nr<=0){
        IupSetAttribute(drop_cp_item_recurso, "1", "(Nenhum)");
        IupSetAttribute(drop_cp_item_recurso, "VALUE", "1");
        return;
    }
    for(int i=0;i<nr;i++){
        char buf[256]; snprintf(buf,sizeof(buf),"%d:%s", recs[i].id, recs[i].descricao);
        IupSetAttributeId(drop_cp_item_recurso, "", i+1, buf);
    }
    IupSetAttribute(drop_cp_item_recurso, "VALUE", "1");
}

static void reload_pagar(void){
    ContaPagar arr[512]; int n=contas_pagar_listar(arr,512); IupSetInt(mat_pagar,"NUMLIN", n>0?n:1); if(n==0){ limpar_matriz(mat_pagar); IupSetAttribute(mat_pagar,"REDRAW","ALL"); return; }
    Fornecedor forn[256]; int nf = fornecedor_listar(forn,256);
    int ocultar = chk_pg_ocultar ? IupGetInt(chk_pg_ocultar, "VALUE") : 0;
    if(n==0){ IupSetInt(mat_pagar,"NUMLIN",1); limpar_matriz(mat_pagar); IupSetAttribute(mat_pagar,"REDRAW","ALL"); return; }
    int count=0; for(int i=0;i<n;i++){ if(ocultar && arr[i].status!=0) continue; count++; }
    if(count==0){ IupSetInt(mat_pagar,"NUMLIN",1); limpar_matriz(mat_pagar); IupSetAttribute(mat_pagar,"REDRAW","ALL"); return; }
    IupSetInt(mat_pagar,"NUMLIN", count);
    int lin_at=1;
    for(int i=0;i<n;i++){
        if(ocultar && arr[i].status!=0) continue;
        IupSetIntId2(mat_pagar,"",lin_at,1, arr[i].id);
        char fdesc[64]=""; for(int j=0;j<nf;j++){ if(forn[j].id==arr[i].fornecedor_id){ strncpy(fdesc, forn[j].nome_fantasia,63); break; } }
        IupSetStrAttributeId2(mat_pagar,"",lin_at,2, fdesc);
        IupSetIntId2(mat_pagar,"",lin_at,3, arr[i].compra_id);
        IupSetStrAttributeId2(mat_pagar,"",lin_at,4, arr[i].data_vencimento);
        IupSetfAttributeId2(mat_pagar,"",lin_at,5,"%.2f", arr[i].valor);
        IupSetStrAttributeId2(mat_pagar,"",lin_at,6, arr[i].status==0?"Aberto":"Pago");
        lin_at++;
    }
    IupSetAttribute(mat_pagar,"REDRAW","ALL");
}

static int btn_cx_lancar_cb(Ihandle *self){
    CaixaLancamento l; memset(&l,0,sizeof(l));
    const char *data_conv = IupGetAttribute(txt_cx_data,"VALUE");
    if(!data_conv || strlen(data_conv)!=10){ IupMessage("Erro","Data inválida."); return IUP_DEFAULT; }
    strncpy(l.data, data_conv,10);
    strncpy(l.descricao, IupGetAttribute(txt_cx_desc,"VALUE"), sizeof(l.descricao)-1);
    l.valor = atof(IupGetAttribute(txt_cx_valor,"VALUE"));
    const char *tipo = IupGetAttribute(drop_cx_tipo,"VALUE");
    const char *forma = IupGetAttribute(drop_cx_forma,"VALUE");
    if(!tipo || !forma || l.valor==0){ IupMessage("Erro","Preencha tipo, forma e valor."); return IUP_DEFAULT; }
    strcpy(l.tipo, atoi(tipo)==1?"ENTRADA":"SAIDA");
    strcpy(l.forma, atoi(forma)==1?"DINHEIRO":(atoi(forma)==2?"DEBITO":"CREDITO"));
    if(strcmp(l.tipo,"ENTRADA")==0 && strcmp(l.forma,"CREDITO")==0){
        IupMessage("Aviso","Pagamento com CARTAO de CREDITO deve ser lançado em Contas a Receber. Use a aba 'Receber' para criar a fatura.");
        return IUP_DEFAULT;
    }
    if(!caixa_lancar(l)) IupMessage("Erro","Falha ao lançar no caixa.");
    reload_caixa();
    return IUP_DEFAULT;
}

static int btn_cr_criar_cb(Ihandle *self){
    int idx = atoi(IupGetAttribute(drop_cr_evento,"VALUE"));
    if(idx<=0){ IupMessage("Erro","Selecione evento."); return IUP_DEFAULT; }
    const char *evtxt = IupGetAttributeId(drop_cr_evento,"", idx);
    int evento_id = evtxt?atoi(evtxt):0;
    double valor = atof(IupGetAttribute(txt_cr_valor,"VALUE"));
    const char *emissao_conv = IupGetAttribute(txt_cr_emissao,"VALUE");
    const char *venc_conv = IupGetAttribute(txt_cr_venc,"VALUE");
    if(!emissao_conv || !venc_conv || strlen(emissao_conv)!=10 || strlen(venc_conv)!=10){ IupMessage("Erro","Datas inválidas."); return IUP_DEFAULT; }
    if(evento_id<=0 || valor<=0){ IupMessage("Erro","Informe evento e valor."); return IUP_DEFAULT; }
    if(!conta_receber_criar(evento_id, valor, emissao_conv, venc_conv)) IupMessage("Erro","Falha ao criar conta a receber.");
    reload_receber();
    return IUP_DEFAULT;
}

static int btn_cr_baixa_cb(Ihandle *self){
    int id = atoi(IupGetAttribute(txt_cr_baixa_id,"VALUE"));
    double valor = atof(IupGetAttribute(txt_cr_baixa_valor,"VALUE"));
    const char *forma_idx = IupGetAttribute(drop_cr_baixa_forma,"VALUE");
    const char *forma = (atoi(forma_idx)==1?"DINHEIRO":(atoi(forma_idx)==2?"DEBITO":"CREDITO"));
    if(id<=0 || valor<=0){ IupMessage("Erro","Informe id e valor."); return IUP_DEFAULT; }
    if(!conta_receber_baixa(id, valor, forma)) IupMessage("Erro","Falha na baixa.");
    reload_receber(); reload_caixa();
    return IUP_DEFAULT;
}

static int btn_cp_add_item_cb(Ihandle *self){
    int ridx = atoi(IupGetAttribute(drop_cp_item_recurso,"VALUE"));
    if(ridx<=0){ IupMessage("Erro","Selecione recurso."); return IUP_DEFAULT; }
    const char *rtext = IupGetAttributeId(drop_cp_item_recurso,"", ridx);
    int recurso_id = rtext?atoi(rtext):0;
    double preco = atof(IupGetAttribute(txt_cp_item_preco,"VALUE"));
    int qtd = atoi(IupGetAttribute(txt_cp_item_qtd,"VALUE"));
    if(recurso_id<=0 || preco<=0 || qtd<=0){ IupMessage("Erro","Item inválido."); return IUP_DEFAULT; }
    int lin = IupGetInt(mat_compra_itens,"NUMLIN"); IupSetInt(mat_compra_itens,"NUMLIN", lin+1); lin++;
    IupSetIntId2(mat_compra_itens,"",lin,1,recurso_id);
    IupSetfAttributeId2(mat_compra_itens,"",lin,2,"%.2f", preco);
    IupSetIntId2(mat_compra_itens,"",lin,3,qtd);
    double frete = atof(IupGetAttribute(txt_cp_frete,"VALUE"));
    double imposto = atof(IupGetAttribute(txt_cp_imposto,"VALUE"));
    Produtora prod; double margem=0.10; if(produtora_obter(&prod)) margem = prod.margem_lucro_padrao;
    int total_lin = IupGetInt(mat_compra_itens,"NUMLIN");
    int total_qtd = 0;
    for(int i=1;i<=total_lin;i++){
        const char *qstr = IupGetAttributeId2(mat_compra_itens,"",i,3);
        if(qstr) total_qtd += atoi(qstr);
    }
    double frete_unit = (total_qtd>0)? frete / total_qtd : 0;
    double imposto_unit = (total_qtd>0)? imposto / total_qtd : 0;
    for(int i=1;i<=total_lin;i++){
        const char *cpre = IupGetAttributeId2(mat_compra_itens,"",i,2);
        const char *cqtd = IupGetAttributeId2(mat_compra_itens,"",i,3);
        if(!cpre||!cqtd) continue;
        double pre = atof(cpre);
        double loc = pre + frete_unit + imposto_unit + (margem * pre);
        IupSetfAttributeId2(mat_compra_itens,"",i,4,"%.2f", loc);
    }
    IupSetAttribute(mat_compra_itens,"REDRAW","ALL");
    return IUP_DEFAULT;
}

static int btn_cp_registrar_cb(Ihandle *self){
    int findx = atoi(IupGetAttribute(drop_cp_forn,"VALUE"));
    if(findx<=0){ IupMessage("Erro","Selecione fornecedor."); return IUP_DEFAULT; }
    const char *ftext = IupGetAttributeId(drop_cp_forn,"", findx);
    const char *data_conv = IupGetAttribute(txt_cp_data,"VALUE");
    if(!data_conv || strlen(data_conv)!=10){ IupMessage("Erro","Data inválida."); return IUP_DEFAULT; }
    Compra c; memset(&c,0,sizeof(c)); c.fornecedor_id = ftext?atoi(ftext):0; strncpy(c.data, data_conv,10); c.frete=atof(IupGetAttribute(txt_cp_frete,"VALUE")); c.imposto=atof(IupGetAttribute(txt_cp_imposto,"VALUE")); c.pagamento_tipo = atoi(IupGetAttribute(drop_cp_pag_tipo,"VALUE"))-1; int parcelas = atoi(IupGetAttribute(txt_cp_parcelas,"VALUE")); if(c.fornecedor_id<=0 || strlen(c.data)<10){ IupMessage("Erro","Fornecedor ou data."); return IUP_DEFAULT; }
    int nlin = IupGetInt(mat_compra_itens,"NUMLIN"); if(nlin<=0){ IupMessage("Erro","Sem itens."); return IUP_DEFAULT; }
    CompraItem itens[256]; int n_it=0; for(int i=1;i<=nlin && n_it<256;i++){ const char *cid = IupGetAttributeId2(mat_compra_itens,"",i,1); const char *cpre = IupGetAttributeId2(mat_compra_itens,"",i,2); const char *cqtd = IupGetAttributeId2(mat_compra_itens,"",i,3); const char *cloc = IupGetAttributeId2(mat_compra_itens,"",i,4); if(!cid||!cpre||!cqtd) continue; int rid=atoi(cid); double pre=atof(cpre); int q=atoi(cqtd); double ploc = cloc?atof(cloc):0; if(rid>0&&pre>0&&q>0){ itens[n_it].recurso_id=rid; itens[n_it].preco_custo_unit=pre; itens[n_it].quantidade=q; itens[n_it].preco_locacao_unit=ploc; n_it++; } }
    if(n_it==0){ IupMessage("Erro","Nenhum item válido."); return IUP_DEFAULT; }
    int cid = compra_registrar(c,itens,n_it, parcelas); if(cid<=0){ IupMessage("Erro","Falha na compra."); } else { IupMessage("Sucesso","Compra registrada."); }
    reload_compras(); reload_pagar(); reload_caixa();
    return IUP_DEFAULT;
}

static int btn_pg_baixar_cb(Ihandle *self){ int id=atoi(IupGetAttribute(txt_pg_id,"VALUE")); if(id<=0){ IupMessage("Erro","Informe ID."); return IUP_DEFAULT; } if(!contas_pagar_baixar(id)) IupMessage("Erro","Falha na baixa."); reload_pagar(); reload_caixa(); return IUP_DEFAULT; }
static int chk_pg_ocultar_cb(Ihandle *self, int state){ (void)self; (void)state; reload_pagar(); return IUP_DEFAULT; }

static Ihandle* build_dialog(void){
    mat_caixa = IupMatrix(NULL); IupSetAttributes(mat_caixa,"NUMCOL=6, NUMLIN=1, RESIZEMATRIX=YES, EXPAND=YES"); IupSetAttributes(mat_caixa,"0:1=ID,0:2=Data,0:3=Tipo,0:4=Forma,0:5=Valor (R$),0:6=Descricao");
    IupSetAttributes(mat_caixa, "WIDTH1=45, WIDTH2=90, WIDTH3=70, WIDTH4=80, WIDTH5=90, WIDTH6=220");
    lbl_saldo = IupLabel("Saldo: R$ 0.00"); IupSetAttribute(lbl_saldo,"FONT","Arial, Bold 12"); IupSetAttributes(lbl_saldo,"RASTERSIZE=260x22, EXPAND=HORIZONTAL");
    txt_cx_data=IupDatePick(); txt_cx_valor=IupText(NULL); txt_cx_desc=IupText(NULL); drop_cx_tipo=IupList(NULL); drop_cx_forma=IupList(NULL);
    IupSetAttributes(drop_cx_tipo,"DROPDOWN=YES"); IupSetAttribute(drop_cx_tipo,"1","ENTRADA"); IupSetAttribute(drop_cx_tipo,"2","SAIDA"); IupSetAttributes(drop_cx_forma,"DROPDOWN=YES"); IupSetAttribute(drop_cx_forma,"1","DINHEIRO"); IupSetAttribute(drop_cx_forma,"2","DEBITO"); IupSetAttribute(drop_cx_forma,"3","CREDITO");
    Ihandle *btn_cx_lancar = IupButton("Lancar", NULL);

    mat_receber = IupMatrix(NULL); IupSetAttributes(mat_receber,"NUMCOL=6, NUMLIN=1, RESIZEMATRIX=YES, EXPAND=YES"); IupSetAttributes(mat_receber,"0:1=ID,0:2=Evento,0:3=Fatura (R$),0:4=Recebido (R$),0:5=Status,0:6=Vencimento");
    IupSetAttributes(mat_receber, "WIDTH1=45, WIDTH2=60, WIDTH3=80, WIDTH4=80, WIDTH5=80, WIDTH6=90");
    drop_cr_evento=IupList(NULL); IupSetAttributes(drop_cr_evento,"DROPDOWN=YES"); txt_cr_valor=IupText(NULL); txt_cr_emissao=IupDatePick(); txt_cr_venc=IupDatePick(); Ihandle *btn_cr_criar = IupButton("Criar Fatura", NULL); txt_cr_baixa_id=IupText(NULL); txt_cr_baixa_valor=IupText(NULL); drop_cr_baixa_forma=IupList(NULL); IupSetAttributes(drop_cr_baixa_forma,"DROPDOWN=YES"); IupSetAttribute(drop_cr_baixa_forma,"1","DINHEIRO"); IupSetAttribute(drop_cr_baixa_forma,"2","DEBITO"); IupSetAttribute(drop_cr_baixa_forma,"3","CREDITO"); Ihandle *btn_cr_baixa = IupButton("Dar Baixa", NULL);

    /* Compras */
    mat_compras = IupMatrix(NULL); 
    IupSetAttributes(mat_compras,"NUMCOL=6, NUMLIN=1, RESIZEMATRIX=YES, EXPAND=YES");
    IupSetAttribute(mat_compras,"0:1","ID");
    IupSetAttribute(mat_compras,"0:2","Fornecedor");
    IupSetAttribute(mat_compras,"0:3","Data");
    IupSetAttribute(mat_compras,"0:4","Total Nota (R$)");
    IupSetAttribute(mat_compras,"0:5","Locacao Media (R$)");
    IupSetAttribute(mat_compras,"0:6","Tipo");
    IupSetAttributes(mat_compras, "WIDTH1=45, WIDTH2=140, WIDTH3=90, WIDTH4=100, WIDTH5=120, WIDTH6=70");
    
    mat_compra_itens = IupMatrix(NULL);
    IupSetAttributes(mat_compra_itens,"NUMCOL=4, NUMLIN=1, RESIZEMATRIX=YES, EXPAND=YES");
    IupSetAttribute(mat_compra_itens,"0:1","RecursoID");
    IupSetAttribute(mat_compra_itens,"0:2","Custo (R$)");
    IupSetAttribute(mat_compra_itens,"0:3","Qtd");
    IupSetAttribute(mat_compra_itens,"0:4","Locacao Unit (R$)");
    IupSetAttribute(mat_compra_itens,"TIP0:4","Formula: Custo + Frete/QtdTotal + Imposto/QtdTotal + Margem*Custo");
    IupSetAttributes(mat_compra_itens, "WIDTH1=80, WIDTH2=90, WIDTH3=50, WIDTH4=120");
    drop_cp_forn=IupList(NULL); IupSetAttributes(drop_cp_forn,"DROPDOWN=YES"); txt_cp_data=IupDatePick(); txt_cp_frete=IupText(NULL); txt_cp_imposto=IupText(NULL); drop_cp_pag_tipo=IupList(NULL); IupSetAttributes(drop_cp_pag_tipo,"DROPDOWN=YES"); IupSetAttribute(drop_cp_pag_tipo,"1","A vista"); IupSetAttribute(drop_cp_pag_tipo,"2","Prazo"); txt_cp_parcelas=IupText(NULL); drop_cp_item_recurso=IupList(NULL); IupSetAttributes(drop_cp_item_recurso,"DROPDOWN=YES"); txt_cp_item_preco=IupText(NULL); txt_cp_item_qtd=IupText(NULL); Ihandle *btn_cp_add_item=IupButton("Add Item", NULL); Ihandle *btn_cp_registrar=IupButton("Registrar Compra", NULL);

    /* Pagar */
    mat_pagar = IupMatrix(NULL); IupSetAttributes(mat_pagar,"NUMCOL=6, NUMLIN=1, RESIZEMATRIX=YES, EXPAND=YES"); IupSetAttributes(mat_pagar,"0:1=ID,0:2=Fornecedor,0:3=Compra,0:4=Vencimento,0:5=Valor (R$),0:6=Status");
    IupSetAttributes(mat_pagar, "WIDTH1=45, WIDTH2=160, WIDTH3=55, WIDTH4=90, WIDTH5=90, WIDTH6=70");
    txt_pg_id=IupText(NULL); Ihandle *btn_pg_baixar=IupButton("Baixar Parcela", NULL);

    IupSetAttributes(txt_cx_data, "RASTERSIZE=110x22");
    IupSetAttributes(drop_cx_tipo, "RASTERSIZE=110x22");
    IupSetAttributes(drop_cx_forma, "RASTERSIZE=110x22");
    IupSetAttributes(txt_cx_valor, "RASTERSIZE=110x22");
    IupSetAttributes(txt_cx_desc, "RASTERSIZE=180x22");
    IupSetAttributes(btn_cx_lancar, "RASTERSIZE=90x26");
    Ihandle *box_cx_form = IupGridBox(
        IupLabel("Data:"), txt_cx_data,
        IupLabel("Tipo:"), drop_cx_tipo,
        IupLabel("Forma:"), drop_cx_forma,
        IupLabel("Valor (R$):"), txt_cx_valor,
        IupLabel("Descricao:"), txt_cx_desc,
        IupFill(), btn_cx_lancar,
        NULL);
    IupSetAttribute(box_cx_form,"NUMCOL","4");
    IupSetAttribute(box_cx_form,"GAPLIN","4");
    IupSetAttribute(box_cx_form,"GAPCOL","6");
    IupSetAttribute(box_cx_form,"MARGIN","4x4");
    Ihandle *tab_caixa = IupVbox(lbl_saldo, mat_caixa, IupFrame(box_cx_form), NULL);

    IupSetAttributes(drop_cr_evento, "RASTERSIZE=180x22");
    IupSetAttributes(txt_cr_valor, "RASTERSIZE=110x22");
    IupSetAttributes(txt_cr_emissao, "RASTERSIZE=110x22");
    IupSetAttributes(txt_cr_venc, "RASTERSIZE=110x22");
    IupSetAttributes(btn_cr_criar, "RASTERSIZE=100x26");
    IupSetAttributes(txt_cr_baixa_id, "RASTERSIZE=70x22");
    IupSetAttributes(txt_cr_baixa_valor, "RASTERSIZE=110x22");
    IupSetAttributes(drop_cr_baixa_forma, "RASTERSIZE=110x22");
    IupSetAttributes(btn_cr_baixa, "RASTERSIZE=100x26");
    Ihandle *box_cr_form = IupGridBox(
        IupLabel("Evento:"), drop_cr_evento,
        IupLabel("Valor (R$):"), txt_cr_valor,
        IupLabel("Emissao:"), txt_cr_emissao,
        IupLabel("Vencimento:"), txt_cr_venc,
        IupFill(), btn_cr_criar,
        IupLabel("BaixaID:"), txt_cr_baixa_id,
        IupLabel("Baixa Valor (R$):"), txt_cr_baixa_valor,
        IupLabel("Forma:"), drop_cr_baixa_forma,
        IupFill(), btn_cr_baixa,
        NULL);
    IupSetAttribute(box_cr_form,"NUMCOL","4");
    IupSetAttribute(box_cr_form,"GAPLIN","4");
    IupSetAttribute(box_cr_form,"GAPCOL","6");
    IupSetAttribute(box_cr_form,"MARGIN","4x4");
    Ihandle *tab_receber = IupVbox(mat_receber, IupFrame(box_cr_form), NULL);

    IupSetAttributes(drop_cp_forn, "RASTERSIZE=180x22");
    IupSetAttributes(txt_cp_data, "RASTERSIZE=110x22");
    IupSetAttributes(txt_cp_frete, "RASTERSIZE=110x22");
    IupSetAttributes(txt_cp_imposto, "RASTERSIZE=110x22");
    IupSetAttributes(drop_cp_pag_tipo, "RASTERSIZE=110x22");
    IupSetAttributes(txt_cp_parcelas, "RASTERSIZE=70x22");
        Ihandle *lbl_fornecedor = IupLabel("Fornecedor:"); IupSetAttributes(lbl_fornecedor, "RASTERSIZE=110x22");
        Ihandle *lbl_data = IupLabel("Data:"); IupSetAttributes(lbl_data, "RASTERSIZE=110x22");
        Ihandle *lbl_frete = IupLabel("Frete (R$):"); IupSetAttributes(lbl_frete, "RASTERSIZE=110x22");
        Ihandle *lbl_imposto = IupLabel("Imposto (R$):"); IupSetAttributes(lbl_imposto, "RASTERSIZE=110x22");
        Ihandle *lbl_tipo_pag = IupLabel("Tipo Pag:"); IupSetAttributes(lbl_tipo_pag, "RASTERSIZE=110x22");
        Ihandle *lbl_parcelas = IupLabel("Parcelas: (Apenas para Prazo)"); IupSetAttributes(lbl_parcelas, "RASTERSIZE=110x22");
        Ihandle *lbl_recurso = IupLabel("Recurso:"); IupSetAttributes(lbl_recurso, "RASTERSIZE=110x22");
        Ihandle *lbl_preco = IupLabel("Custo (R$):"); IupSetAttributes(lbl_preco, "RASTERSIZE=110x22");
        Ihandle *lbl_qtd = IupLabel("Qtd:"); IupSetAttributes(lbl_qtd, "RASTERSIZE=110x22");
        Ihandle *box_cp_form = IupGridBox(
            lbl_fornecedor, drop_cp_forn,
            lbl_data, txt_cp_data,
            lbl_frete, txt_cp_frete,
            lbl_imposto, txt_cp_imposto,
            lbl_tipo_pag, drop_cp_pag_tipo,
            lbl_parcelas, txt_cp_parcelas,
            lbl_recurso, drop_cp_item_recurso,
            lbl_preco, txt_cp_item_preco,
            lbl_qtd, txt_cp_item_qtd,
            IupFill(), btn_cp_add_item,
            NULL);
    IupSetAttribute(box_cp_form,"NUMCOL","4"); IupSetAttribute(box_cp_form,"GAPLIN","4"); IupSetAttribute(box_cp_form,"GAPCOL","6"); IupSetAttribute(box_cp_form,"MARGIN","4x4");
    IupSetAttributes(btn_cp_registrar, "RASTERSIZE=130x28");
    Ihandle *tab_compras = IupVbox(mat_compras, IupFrame(box_cp_form), IupFrame(mat_compra_itens), btn_cp_registrar, NULL);

    IupSetAttributes(txt_pg_id, "RASTERSIZE=80x22"); IupSetAttributes(btn_pg_baixar, "RASTERSIZE=120x26");
        chk_pg_ocultar = IupToggle("Ocultar Pagas", NULL); IupSetAttributes(chk_pg_ocultar, "RASTERSIZE=130x22");
        Ihandle *lbl_parcela_id = IupLabel("Parcela ID:"); IupSetAttributes(lbl_parcela_id, "RASTERSIZE=110x22");
        Ihandle *box_pg = IupGridBox(lbl_parcela_id, txt_pg_id, chk_pg_ocultar, btn_pg_baixar, NULL); IupSetAttribute(box_pg,"NUMCOL","4"); IupSetAttribute(box_pg,"GAPLIN","4"); IupSetAttribute(box_pg,"GAPCOL","6"); IupSetAttribute(box_pg,"MARGIN","4x4"); Ihandle *tab_pagar = IupVbox(mat_pagar, IupFrame(box_pg), NULL);

    Ihandle *tabs = IupTabs(tab_caixa, tab_receber, tab_compras, tab_pagar, NULL); IupSetAttribute(tab_caixa,"TABTITLE","Caixa"); IupSetAttribute(tab_receber,"TABTITLE","Receber"); IupSetAttribute(tab_compras,"TABTITLE","Compras"); IupSetAttribute(tab_pagar,"TABTITLE","Pagar");

    IupSetCallback(btn_cx_lancar,"ACTION", (Icallback)btn_cx_lancar_cb);
    IupSetCallback(btn_cr_criar,"ACTION", (Icallback)btn_cr_criar_cb);
    IupSetCallback(btn_cr_baixa,"ACTION", (Icallback)btn_cr_baixa_cb);
    IupSetCallback(btn_cp_add_item,"ACTION", (Icallback)btn_cp_add_item_cb);
    IupSetCallback(btn_cp_registrar,"ACTION", (Icallback)btn_cp_registrar_cb);
    IupSetCallback(btn_pg_baixar,"ACTION", (Icallback)btn_pg_baixar_cb);
        IupSetCallback(btn_pg_baixar,"ACTION", (Icallback)btn_pg_baixar_cb);
        IupSetCallback(chk_pg_ocultar,"ACTION", (Icallback)chk_pg_ocultar_cb);
    IupSetCallback(mat_compras, "CLICK_CB", (Icallback)mat_compras_click_cb);

    return tabs;
}

void transacoes_view_mostrar(void){
    if(!dlg){ dlg = IupDialog(build_dialog()); IupSetAttribute(dlg,"TITLE","Transacoes"); IupSetAttribute(dlg,"SIZE","900x550"); IupSetAttribute(dlg,"RESIZE","YES"); IupSetAttribute(dlg,"MAXBOX","YES"); }
    compra_reprocessar_locacoes();
    reload_caixa(); reload_receber(); reload_compras(); reload_pagar(); reload_list_eventos(); reload_list_fornecedores(); reload_list_recursos();
    const char *first_id = IupGetAttributeId2(mat_compras, "", 1, 1);
    if(first_id){ int cid = atoi(first_id); if(cid>0) carregar_itens_compra(cid); }
    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
}
