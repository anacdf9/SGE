#include <iup.h>
#include <iupcontrols.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../controller/evento_controller.h"
#include "../controller/cliente_controller.h"
#include "../controller/recurso_controller.h"
#include "../controller/equipe_controller.h"
#include "../controller/fornecedor_controller.h"
#include "../model/evento.h"
#include "../model/cliente.h"
#include "ui_common.h"

static Ihandle *dlg_evento = NULL;
static Ihandle *mat_evento = NULL;

/* forward decl. para ordenação de clientes usada antes da definição */
static int cliente_nome_cmp(const void *a,const void *b);

static const char* iso_to_br(const char *s){
  static char out[11];
  if(!s||strlen(s)<8){ strcpy(out,"--/--/----"); return out; }
  int a=0,b=0,c=0;
  if(strchr(s,'-')){
    if(sscanf(s, "%d-%d-%d", &a, &b, &c)==3){ /* assume y-m-d */
      snprintf(out, sizeof(out), "%02d/%02d/%04d", c, b, a);
      return out;
    }
  } else if(strchr(s,'/')){
    if(sscanf(s, "%d/%d/%d", &a, &b, &c)==3){
      if(a>31){ /* yyyy/mm/dd */
        snprintf(out, sizeof(out), "%02d/%02d/%04d", c, b, a);
      } else { /* dd/mm/yyyy */
        snprintf(out, sizeof(out), "%02d/%02d/%04d", a, b, c);
      }
      return out;
    }
  }
  strcpy(out,"--/--/----"); return out;
}
/* helper: carregar evento por ID da persistência */
static int evento_carregar_por_id(int id, Evento *out){
  if(id<=0||!out) return 0;
  Evento buf[512]; int n=evento_listar(buf,512);
  for(int i=0;i<n;i++){ if(buf[i].id==id){ *out=buf[i]; return 1; } }
  return 0;
}
static int date_cmp(int d1,int m1,int y1,int d2,int m2,int y2){ if(y1!=y2) return y1<y2?-1:1; if(m1!=m2) return m1<m2?-1:1; if(d1!=d2) return d1<d2?-1:1; return 0; }
/* aceita dd/mm/yyyy, yyyy-mm-dd e yyyy/mm/dd */
static int parse_date_auto(const char *s,int *d,int *m,int *y){
  if(!s) return 0;
  if(strchr(s,'-')){
    int yy,mm,dd; if(sscanf(s,"%d-%d-%d",&yy,&mm,&dd)==3){ *d=dd; *m=mm; *y=yy; return 1; }
  } else if(strchr(s,'/')){
    int a,b,c; if(sscanf(s,"%d/%d/%d",&a,&b,&c)==3){ if(a>31){ /* yyyy/mm/dd */ *y=a; *m=b; *d=c; } else { /* dd/mm/yyyy */ *d=a; *m=b; *y=c; } return 1; }
  }
  return 0;
}
static void evento_view_recarregar(Ihandle *mat){ Evento buf[256]; int n=evento_listar(buf,256); IupSetAttribute(mat,"NUMCOL","10");
 IupSetAttribute(mat,"0:1","ID"); IupSetAttribute(mat,"0:2","Cliente"); IupSetAttribute(mat,"0:3","Recurso"); IupSetAttribute(mat,"0:4","Equipe"); IupSetAttribute(mat,"0:5","Fornecedor"); IupSetAttribute(mat,"0:6","Início"); IupSetAttribute(mat,"0:7","Fim"); IupSetAttribute(mat,"0:8","Status"); IupSetAttribute(mat,"0:9","Estimado"); IupSetAttribute(mat,"0:10","Final");
 /* filtros */
 Ihandle *fIni=IupGetAttributeHandle(mat,"txtFiltroInicio"); Ihandle *fFim=IupGetAttributeHandle(mat,"txtFiltroFim");
 int fd=0,fm=0,fy=0,td=0,tm=0,ty=0; int usar_filtro_data=0; if(fIni&&fFim){ const char *sv1=IupGetAttribute(fIni,"VALUE"); const char *sv2=IupGetAttribute(fFim,"VALUE"); if(parse_date_auto(sv1,&fd,&fm,&fy) && parse_date_auto(sv2,&td,&tm,&ty) && date_cmp(fd,fm,fy,td,tm,ty)<=0){ usar_filtro_data=1; } }
 /* carrega listas */
 Cliente clientes[256]; int cn=cliente_listar(clientes,256);
 Recurso recursos[256]; int rn=recurso_listar(recursos,256);
 Equipe equipes[256]; int en=equipe_listar(equipes,256);
 Fornecedor forns[256]; int fn=fornecedor_listar(forns,256);
 int linha_visual=0;
 for(int i=0;i<n;i++){
  if(usar_filtro_data){ int d1,m1,y1; if(!parse_date_auto(buf[i].data_inicio,&d1,&m1,&y1)) continue; if(date_cmp(d1,m1,y1,fd,fm,fy)<0 || date_cmp(d1,m1,y1,td,tm,ty)>0) continue; }
  linha_visual++; char idb[32]; sprintf(idb,"%d",buf[i].id); char estb[32]; sprintf(estb,"%.2f",buf[i].total_estimado); char finb[32]; sprintf(finb,"%.2f",buf[i].total_final); char st[32];
  if(buf[i].status==EVENTO_STATUS_RASCUNHO) strcpy(st,"Rascunho");
  else if(buf[i].status==EVENTO_STATUS_APROVADO) strcpy(st,"Aprovado");
  else if(buf[i].status==EVENTO_STATUS_FINALIZADO) strcpy(st,"Finalizado");
  else snprintf(st,sizeof(st),"%d", (int)buf[i].status);
  char cname[128]; cname[0]='\0'; for(int c=0;c<cn;c++){ if(clientes[c].id==buf[i].cliente_id){ snprintf(cname,sizeof(cname),"%d - %s", clientes[c].id, clientes[c].nome); break; } }
  if(cname[0]=='\0') strcpy(cname,"(sem cliente)");
  char rname[140]; rname[0]='\0'; for(int r=0;r<rn;r++){ if(recursos[r].id==buf[i].recurso_id){ snprintf(rname,sizeof(rname),"%d - %s", recursos[r].id, recursos[r].descricao); break; } }
  if(rname[0]=='\0') sprintf(rname,"%d", buf[i].recurso_id);
  char ename[140]; ename[0]='\0'; for(int e=0;e<en;e++){ if(equipes[e].id==buf[i].equipe_id){ snprintf(ename,sizeof(ename),"%d - %s", equipes[e].id, equipes[e].nome); break; } }
  if(ename[0]=='\0') sprintf(ename,"%d", buf[i].equipe_id);
  char fname[140]; fname[0]='\0'; for(int f=0;f<fn;f++){ if(forns[f].id==buf[i].fornecedor_id){ snprintf(fname,sizeof(fname),"%d - %s", forns[f].id, forns[f].nome_fantasia); break; } }
  if(fname[0]=='\0') sprintf(fname,"%d", buf[i].fornecedor_id);
  IupSetStrAttributeId2(mat,"",linha_visual,1,idb);
  IupSetStrAttributeId2(mat,"",linha_visual,2,cname);
  IupSetStrAttributeId2(mat,"",linha_visual,3,rname);
  IupSetStrAttributeId2(mat,"",linha_visual,4,ename);
  IupSetStrAttributeId2(mat,"",linha_visual,5,fname);
  IupSetStrAttributeId2(mat,"",linha_visual,6,(char*)iso_to_br(buf[i].data_inicio));
  IupSetStrAttributeId2(mat,"",linha_visual,7,(char*)iso_to_br(buf[i].data_fim));
  IupSetStrAttributeId2(mat,"",linha_visual,8,st); IupSetStrAttributeId2(mat,"",linha_visual,9,estb); IupSetStrAttributeId2(mat,"",linha_visual,10,finb);
 }
 if(linha_visual==0){ linha_visual=1; }
 char k2[16]; sprintf(k2,"%d",linha_visual); IupSetAttribute(mat,"NUMLIN",k2);
}

/* Recarrega as listas de seleção (cliente, recurso, equipe, fornecedor) e o filtro de cliente */
static void evento_repopular_listas(Ihandle *anchor){
  if(!anchor) return;
  Ihandle *txtCliente=IupGetAttributeHandle(anchor,"txtCliente");
  Ihandle *txtRecurso=IupGetAttributeHandle(anchor,"txtRecurso");
  Ihandle *txtEquipe=IupGetAttributeHandle(anchor,"txtEquipe");
  Ihandle *txtFornecedor=IupGetAttributeHandle(anchor,"txtFornecedor");
  /* guarda seleções atuais (IDs) para restaurar após repopular */
  int sel_cli_id=0, sel_rec_id=0, sel_eq_id=0, sel_for_id=0;
  if(txtCliente){ int idx=atoi(IupGetAttribute(txtCliente,"VALUE")); if(idx>0){ const char *it=IupGetAttributeId(txtCliente,"",idx); if(it) sel_cli_id=atoi(it); }}
  if(txtRecurso){ int idx=atoi(IupGetAttribute(txtRecurso,"VALUE")); if(idx>0){ const char *it=IupGetAttributeId(txtRecurso,"",idx); if(it) sel_rec_id=atoi(it); }}
  if(txtEquipe){ int idx=atoi(IupGetAttribute(txtEquipe,"VALUE")); if(idx>0){ const char *it=IupGetAttributeId(txtEquipe,"",idx); if(it) sel_eq_id=atoi(it); }}
  if(txtFornecedor){ int idx=atoi(IupGetAttribute(txtFornecedor,"VALUE")); if(idx>0){ const char *it=IupGetAttributeId(txtFornecedor,"",idx); if(it) sel_for_id=atoi(it); }}

  /* Clientes (ordenado) */
  if(txtCliente){
    Cliente cbuf[256]; int cn=cliente_listar(cbuf,256); if(cn>1) qsort(cbuf,cn,sizeof(Cliente),cliente_nome_cmp);
    IupSetAttribute(txtCliente,"REMOVEITEM","ALL");
    if(cn<=0){ IupSetStrAttributeId(txtCliente, "", 1, "0 - (nenhum)"); }
    else { for(int i=0;i<cn;i++){ char line[256]; snprintf(line,sizeof(line),"%d - %s", cbuf[i].id, cbuf[i].nome); IupSetStrAttributeId(txtCliente, "", i+1, line); } }
    /* restaura seleção por ID */
    if(sel_cli_id>0){ int count=atoi(IupGetAttribute(txtCliente,"COUNT")); for(int i=1;i<=count;i++){ const char *it=IupGetAttributeId(txtCliente,"",i); if(it && atoi(it)==sel_cli_id){ char b[16]; snprintf(b,sizeof(b),"%d",i); IupSetAttribute(txtCliente,"VALUE",b); break; } } }
    if(!atoi(IupGetAttribute(txtCliente,"VALUE"))) IupSetAttribute(txtCliente,"VALUE","1");
  }

  /* Recursos */
  if(txtRecurso){ Recurso rbuf[256]; int rn=recurso_listar(rbuf,256); IupSetAttribute(txtRecurso,"REMOVEITEM","ALL"); if(rn<=0){ IupSetStrAttributeId(txtRecurso, "", 1, "0 - (nenhum)"); } else { for(int i=0;i<rn;i++){ char line[200]; snprintf(line,sizeof(line),"%d - %s", rbuf[i].id, rbuf[i].descricao); IupSetStrAttributeId(txtRecurso, "", i+1, line); } } if(sel_rec_id>0){ int count=atoi(IupGetAttribute(txtRecurso,"COUNT")); for(int i=1;i<=count;i++){ const char *it=IupGetAttributeId(txtRecurso,"",i); if(it && atoi(it)==sel_rec_id){ char b[16]; snprintf(b,sizeof(b),"%d",i); IupSetAttribute(txtRecurso,"VALUE",b); break; } } } if(!atoi(IupGetAttribute(txtRecurso,"VALUE"))) IupSetAttribute(txtRecurso,"VALUE","1"); }

  /* Equipes */
  if(txtEquipe){ Equipe ebuf[256]; int en=equipe_listar(ebuf,256); IupSetAttribute(txtEquipe,"REMOVEITEM","ALL"); if(en<=0){ IupSetStrAttributeId(txtEquipe, "", 1, "0 - (nenhuma)"); } else { for(int i=0;i<en;i++){ char line[200]; snprintf(line,sizeof(line),"%d - %s", ebuf[i].id, ebuf[i].nome); IupSetStrAttributeId(txtEquipe, "", i+1, line); } } if(sel_eq_id>0){ int count=atoi(IupGetAttribute(txtEquipe,"COUNT")); for(int i=1;i<=count;i++){ const char *it=IupGetAttributeId(txtEquipe,"",i); if(it && atoi(it)==sel_eq_id){ char b[16]; snprintf(b,sizeof(b),"%d",i); IupSetAttribute(txtEquipe,"VALUE",b); break; } } } if(!atoi(IupGetAttribute(txtEquipe,"VALUE"))) IupSetAttribute(txtEquipe,"VALUE","1"); }

  /* Fornecedores */
  if(txtFornecedor){ Fornecedor fbuf[256]; int fn=fornecedor_listar(fbuf,256); IupSetAttribute(txtFornecedor,"REMOVEITEM","ALL"); if(fn<=0){ IupSetStrAttributeId(txtFornecedor, "", 1, "0 - (nenhum)"); } else { for(int i=0;i<fn;i++){ char line[200]; snprintf(line,sizeof(line),"%d - %s", fbuf[i].id, fbuf[i].nome_fantasia); IupSetStrAttributeId(txtFornecedor, "", i+1, line); } } if(sel_for_id>0){ int count=atoi(IupGetAttribute(txtFornecedor,"COUNT")); for(int i=1;i<=count;i++){ const char *it=IupGetAttributeId(txtFornecedor,"",i); if(it && atoi(it)==sel_for_id){ char b[16]; snprintf(b,sizeof(b),"%d",i); IupSetAttribute(txtFornecedor,"VALUE",b); break; } } } if(!atoi(IupGetAttribute(txtFornecedor,"VALUE"))) IupSetAttribute(txtFornecedor,"VALUE","1"); }
}

static int evento_click_cb(Ihandle *mat,int lin,int col,char *status){ if(lin<=0) return IUP_DEFAULT; Ihandle *tId=IupGetAttributeHandle(mat,"txtId"); Ihandle *tCliente=IupGetAttributeHandle(mat,"txtCliente"); Ihandle *tRecurso=IupGetAttributeHandle(mat,"txtRecurso"); Ihandle *tEquipe=IupGetAttributeHandle(mat,"txtEquipe"); Ihandle *tForn=IupGetAttributeHandle(mat,"txtFornecedor"); Ihandle *tIni=IupGetAttributeHandle(mat,"txtInicio"); Ihandle *tFim=IupGetAttributeHandle(mat,"txtFim"); Ihandle *tRecursoQtd=IupGetAttributeHandle(mat,"txtRecursoQtd"); Ihandle *tEquipeHoras=IupGetAttributeHandle(mat,"txtEquipeHoras"); Ihandle *tFornValor=IupGetAttributeHandle(mat,"txtFornecedorValor"); Ihandle *tLocal=IupGetAttributeHandle(mat,"txtLocal");
 const char *v; v=IupGetAttributeId2(mat,"",lin,1); IupSetStrAttribute(tId,"VALUE", v?v:"");
 const char *c=IupGetAttributeId2(mat,"",lin,2); if(c&&tCliente){ int cid=atoi(c); int count=atoi(IupGetAttribute(tCliente,"COUNT")); for(int i=1;i<=count;i++){ const char *item=IupGetAttributeId(tCliente,"",i); if(item && atoi(item)==cid){ char idx[16]; snprintf(idx,sizeof(idx),"%d",i); IupSetAttribute(tCliente,"VALUE",idx); break; } } }
 const char *r=IupGetAttributeId2(mat,"",lin,3); if(r&&tRecurso){ int rid=atoi(r); int count=atoi(IupGetAttribute(tRecurso,"COUNT")); for(int i=1;i<=count;i++){ const char *item=IupGetAttributeId(tRecurso,"",i); if(item && atoi(item)==rid){ char idx[16]; snprintf(idx,sizeof(idx),"%d",i); IupSetAttribute(tRecurso,"VALUE",idx); break; } } }
 const char *e=IupGetAttributeId2(mat,"",lin,4); if(e&&tEquipe){ int eid=atoi(e); int count=atoi(IupGetAttribute(tEquipe,"COUNT")); for(int i=1;i<=count;i++){ const char *item=IupGetAttributeId(tEquipe,"",i); if(item && atoi(item)==eid){ char idx[16]; snprintf(idx,sizeof(idx),"%d",i); IupSetAttribute(tEquipe,"VALUE",idx); break; } } }
 const char *f=IupGetAttributeId2(mat,"",lin,5); if(f&&tForn){ int fid=atoi(f); int count=atoi(IupGetAttribute(tForn,"COUNT")); for(int i=1;i<=count;i++){ const char *item=IupGetAttributeId(tForn,"",i); if(item && atoi(item)==fid){ char idx[16]; snprintf(idx,sizeof(idx),"%d",i); IupSetAttribute(tForn,"VALUE",idx); break; } } }
 /* datas: converter da célula (dd/mm/yyyy) para yyyy/mm/dd antes de setar no DatePick */
 const char *vin=IupGetAttributeId2(mat,"",lin,6); int dd,mm,yy; if(parse_date_auto(vin,&dd,&mm,&yy)){ char iso[32]; snprintf(iso,sizeof(iso),"%04d/%02d/%02d", yy,mm,dd); IupSetAttribute(tIni,"VALUE", iso); } else { IupSetAttribute(tIni,"VALUE",""); }
 const char *vfi=IupGetAttributeId2(mat,"",lin,7); if(parse_date_auto(vfi,&dd,&mm,&yy)){ char iso2[32]; snprintf(iso2,sizeof(iso2),"%04d/%02d/%02d", yy,mm,dd); IupSetAttribute(tFim,"VALUE", iso2); } else { IupSetAttribute(tFim,"VALUE",""); }
 /* carregar campos adicionais não exibidos na matriz */
 int cur_id = v?atoi(v):0; Evento ev; if(evento_carregar_por_id(cur_id,&ev)){
   if(tRecursoQtd){ char b[32]; snprintf(b,sizeof(b),"%d", ev.recurso_qtd); IupSetAttribute(tRecursoQtd,"VALUE", b); }
   if(tEquipeHoras){ char b[32]; snprintf(b,sizeof(b),"%.2f", ev.equipe_horas); IupSetAttribute(tEquipeHoras,"VALUE", b); }
   if(tFornValor){ char b[32]; snprintf(b,sizeof(b),"%.2f", ev.fornecedor_valor); IupSetAttribute(tFornValor,"VALUE", b); }
   if(tLocal){ IupSetAttribute(tLocal,"VALUE", ev.local); }
 }
 return IUP_DEFAULT; }

static int evento_novo_cb(Ihandle *self){ const char *ids[]={"txtId","txtCliente","txtRecurso","txtRecursoQtd","txtEquipe","txtEquipeHoras","txtFornecedor","txtFornecedorValor","txtInicio","txtFim","txtLocal"}; for(int i=0;i<11;i++){ Ihandle *h=IupGetAttributeHandle(self,ids[i]); if(h) IupSetAttribute(h,"VALUE",""); } Ihandle *focus=IupGetAttributeHandle(self,"txtCliente"); if(focus) IupSetFocus(focus); return IUP_DEFAULT; }

static int evento_salvar_cb(Ihandle *self){ Ihandle *tId=IupGetAttributeHandle(self,"txtId"); Ihandle *tCliente=IupGetAttributeHandle(self,"txtCliente"); Ihandle *tRecurso=IupGetAttributeHandle(self,"txtRecurso"); Ihandle *tQtd=IupGetAttributeHandle(self,"txtRecursoQtd"); Ihandle *tEquipe=IupGetAttributeHandle(self,"txtEquipe"); Ihandle *tHoras=IupGetAttributeHandle(self,"txtEquipeHoras"); Ihandle *tForn=IupGetAttributeHandle(self,"txtFornecedor"); Ihandle *tFval=IupGetAttributeHandle(self,"txtFornecedorValor"); Ihandle *tIni=IupGetAttributeHandle(self,"txtInicio"); Ihandle *tFim=IupGetAttributeHandle(self,"txtFim"); Ihandle *tLocal=IupGetAttributeHandle(self,"txtLocal");
 Evento e; memset(&e,0,sizeof(e)); e.id=atoi(IupGetAttribute(tId,"VALUE"));
 /* Se o ID estiver vazio, tenta obter da linha focada na matriz para atualizar em vez de criar novo */
 if(e.id<=0 && mat_evento){ int fl=atoi(IupGetAttribute(mat_evento,"FOCUS_LIN")); if(fl>0){ const char *c1=IupGetAttributeId2(mat_evento,"",fl,1); if(c1 && *c1) e.id=atoi(c1); } }
  Evento antigo; int tem_antigo = evento_carregar_por_id(e.id,&antigo);
 int sel_cliente=atoi(IupGetAttribute(tCliente,"VALUE")); if(sel_cliente>0){ const char *txt=IupGetAttributeId(tCliente,"",sel_cliente); if(txt) e.cliente_id=atoi(txt); }
 int sel_recurso=atoi(IupGetAttribute(tRecurso,"VALUE")); if(sel_recurso>0){ const char *txt=IupGetAttributeId(tRecurso,"",sel_recurso); if(txt) e.recurso_id=atoi(txt); }
 int sel_equipe=atoi(IupGetAttribute(tEquipe,"VALUE")); if(sel_equipe>0){ const char *txt=IupGetAttributeId(tEquipe,"",sel_equipe); if(txt) e.equipe_id=atoi(txt); }
 int sel_forn=atoi(IupGetAttribute(tForn,"VALUE")); if(sel_forn>0){ const char *txt=IupGetAttributeId(tForn,"",sel_forn); if(txt) e.fornecedor_id=atoi(txt); }
 e.recurso_qtd=atoi(IupGetAttribute(tQtd,"VALUE")); e.equipe_horas=atof(IupGetAttribute(tHoras,"VALUE")); e.fornecedor_valor=atof(IupGetAttribute(tFval,"VALUE"));
 /* Converte datas (aceita dd/mm/yyyy ou yyyy-mm-dd) -> YYYY-MM-DD */
 const char *dini=IupGetAttribute(tIni,"VALUE"); const char *dfim=IupGetAttribute(tFim,"VALUE");
 int d1,m1,y1,d2,m2,y2; if(!parse_date_auto(dini,&d1,&m1,&y1)){ IupMessage("Erro","Data início inválida."); return IUP_DEFAULT; }
 if(!parse_date_auto(dfim,&d2,&m2,&y2)){ IupMessage("Erro","Data fim inválida."); return IUP_DEFAULT; }
 /* valida limites simples */
 if(y1>y2 || (y1==y2 && m1>m2) || (y1==y2 && m1==m2 && d1>d2)){ IupMessage("Erro","Data início maior que data fim."); return IUP_DEFAULT; }
 snprintf(e.data_inicio,11,"%04d-%02d-%02d",y1,m1,d1); snprintf(e.data_fim,11,"%04d-%02d-%02d",y2,m2,d2);
 const char *locval=IupGetAttribute(tLocal,"VALUE"); if(!locval) locval=""; strncpy(e.local,locval,119); e.local[119]='\0';
  /* preservar status anterior se editando */
  if(e.id>0 && tem_antigo){ e.status = antigo.status; } else { e.status=EVENTO_STATUS_RASCUNHO; }
  int ok=evento_salvar(e); if(ok){ IupSetfAttribute(tId,"VALUE","%d",e.id); IupMessage("Sucesso", e.id>0 && tem_antigo ? "Orçamento atualizado." : "Orçamento criado."); } else IupMessage("Erro","Falha ao salvar."); if(mat_evento){ evento_view_recarregar(mat_evento); /* foca na linha do ID salvo */
   int linhas=atoi(IupGetAttribute(mat_evento,"NUMLIN")); char idb[32]; snprintf(idb,sizeof(idb),"%d", e.id); for(int i=1;i<=linhas;i++){ const char *c1=IupGetAttributeId2(mat_evento,"",i,1); if(c1 && strcmp(c1,idb)==0){ char bi[16]; snprintf(bi,sizeof(bi),"%d", i); IupSetAttribute(mat_evento,"FOCUS_LIN", bi); break; } } }
 return IUP_DEFAULT; }

static int evento_aprovar_cb(Ihandle *self){ Ihandle *tId=IupGetAttributeHandle(self,"txtId"); int id=atoi(IupGetAttribute(tId,"VALUE")); if(id<=0){ IupMessage("Erro","ID inválido."); return IUP_DEFAULT; } if(evento_aprovar(id)) IupMessage("Sucesso","Evento aprovado."); else IupMessage("Erro","Não foi possível aprovar. Verifique datas."); if(mat_evento) evento_view_recarregar(mat_evento); return IUP_DEFAULT; }
static int evento_finalizar_cb(Ihandle *self){ Ihandle *tId=IupGetAttributeHandle(self,"txtId"); int id=atoi(IupGetAttribute(tId,"VALUE")); if(id<=0){ IupMessage("Erro","ID inválido."); return IUP_DEFAULT; } if(evento_finalizar(id)) IupMessage("Sucesso","Evento finalizado."); else IupMessage("Erro","Status inválido."); if(mat_evento) evento_view_recarregar(mat_evento); return IUP_DEFAULT; }
static int evento_atualizar_cb(Ihandle *self){
  /* Atualiza listas e matriz para refletir alterações externas (novos cadastros/edições) */
  if(mat_evento) evento_repopular_listas(mat_evento);
  if(mat_evento) evento_view_recarregar(mat_evento);
  return IUP_DEFAULT;
}
/* callback para mudança imediata nos filtros (lista) */
/* filtros de Status/Cliente removidos: callback de filtro não é mais necessário */
static int evento_excluir_cb(Ihandle *self){ Ihandle *tId=IupGetAttributeHandle(self,"txtId"); int id=atoi(IupGetAttribute(tId,"VALUE")); if(id<=0){ IupMessage("Erro","Selecione um orçamento válido para excluir."); return IUP_DEFAULT; } if(IupAlarm("Confirmação","Excluir este orçamento?","OK","Cancelar",NULL)==1){ if(evento_excluir(id)){ IupMessage("Sucesso","Orçamento excluído."); IupSetAttribute(tId,"VALUE",""); if(mat_evento) evento_view_recarregar(mat_evento); } else { IupMessage("Erro","Falha ao excluir orçamento."); } } return IUP_DEFAULT; }
static int evento_exportar_cb(Ihandle *self){ if(!mat_evento){ IupMessage("Erro","Sem dados."); return IUP_DEFAULT; } evento_view_recarregar(mat_evento); FILE *f=fopen("eventos_export.csv","w"); if(!f){ IupMessage("Erro","Falha ao criar arquivo."); return IUP_DEFAULT; } int linhas=atoi(IupGetAttribute(mat_evento,"NUMLIN")); fprintf(f,"ID;Cliente;Recurso;Equipe;Fornecedor;Inicio;Fim;Status;Estimado;Final\n"); for(int i=1;i<=linhas;i++){ const char *c1=IupGetAttributeId2(mat_evento,"",i,1); if(!c1||!*c1) continue; const char *c2=IupGetAttributeId2(mat_evento,"",i,2); const char *c3=IupGetAttributeId2(mat_evento,"",i,3); const char *c4=IupGetAttributeId2(mat_evento,"",i,4); const char *c5=IupGetAttributeId2(mat_evento,"",i,5); const char *c6=IupGetAttributeId2(mat_evento,"",i,6); const char *c7=IupGetAttributeId2(mat_evento,"",i,7); const char *c8=IupGetAttributeId2(mat_evento,"",i,8); const char *c9=IupGetAttributeId2(mat_evento,"",i,9); const char *c10=IupGetAttributeId2(mat_evento,"",i,10); fprintf(f,"%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",c1,c2?c2:"",c3?c3:"",c4?c4:"",c5?c5:"",c6?c6:"",c7?c7:"",c8?c8:"",c9?c9:"",c10?c10:""); } fclose(f); IupMessage("Exportação","Gerado eventos_export.csv."); return IUP_DEFAULT; }

static int cliente_nome_cmp(const void *a,const void *b){ const Cliente *c1=(const Cliente*)a; const Cliente *c2=(const Cliente*)b; return strcmp(c1->nome,c2->nome); }
static Ihandle* evento_view_create(void){ Ihandle *txtId=IupText(NULL); IupSetAttribute(txtId,"READONLY","YES");
 /* Dropdown de clientes (ordenado) */
 Ihandle *txtCliente=IupList(NULL); IupSetAttribute(txtCliente,"DROPDOWN","YES"); IupSetAttribute(txtCliente,"EDITBOX","NO");
 Cliente cbuf[256]; int cn=cliente_listar(cbuf,256); if(cn>1) qsort(cbuf,cn,sizeof(Cliente),cliente_nome_cmp);
 IupSetAttribute(txtCliente,"REMOVEITEM","ALL");
 if(cn<=0){ IupSetStrAttributeId(txtCliente, "", 1, "0 - (nenhum)"); }
 else { for(int i=0;i<cn;i++){ char line[256]; snprintf(line,sizeof(line),"%d - %s", cbuf[i].id, cbuf[i].nome); IupSetStrAttributeId(txtCliente, "", i+1, line); } }
 IupSetAttribute(txtCliente,"VALUE","1"); /* seleciona primeira entrada */
 /* Campos restantes convertidos para listas */
 Ihandle *txtRecurso=IupList(NULL); IupSetAttribute(txtRecurso,"DROPDOWN","YES"); IupSetAttribute(txtRecurso,"EDITBOX","NO");
 Recurso rbuf[256]; int rn=recurso_listar(rbuf,256); IupSetAttribute(txtRecurso,"REMOVEITEM","ALL"); if(rn<=0){ IupSetStrAttributeId(txtRecurso, "", 1, "0 - (nenhum)"); } else { for(int i=0;i<rn;i++){ char line[200]; snprintf(line,sizeof(line),"%d - %s", rbuf[i].id, rbuf[i].descricao); IupSetStrAttributeId(txtRecurso, "", i+1, line); } }
 IupSetAttribute(txtRecurso,"VALUE","1");
 Ihandle *txtRecursoQtd=IupText(NULL);
 Ihandle *txtEquipe=IupList(NULL); IupSetAttribute(txtEquipe,"DROPDOWN","YES"); IupSetAttribute(txtEquipe,"EDITBOX","NO");
 Equipe ebuf[256]; int en=equipe_listar(ebuf,256); IupSetAttribute(txtEquipe,"REMOVEITEM","ALL"); if(en<=0){ IupSetStrAttributeId(txtEquipe, "", 1, "0 - (nenhuma)"); } else { for(int i=0;i<en;i++){ char line[200]; snprintf(line,sizeof(line),"%d - %s", ebuf[i].id, ebuf[i].nome); IupSetStrAttributeId(txtEquipe, "", i+1, line); } }
 IupSetAttribute(txtEquipe,"VALUE","1");
 Ihandle *txtEquipeHoras=IupText(NULL);
 Ihandle *txtFornecedor=IupList(NULL); IupSetAttribute(txtFornecedor,"DROPDOWN","YES"); IupSetAttribute(txtFornecedor,"EDITBOX","NO");
 Fornecedor fbuf[256]; int fn=fornecedor_listar(fbuf,256); IupSetAttribute(txtFornecedor,"REMOVEITEM","ALL"); if(fn<=0){ IupSetStrAttributeId(txtFornecedor, "", 1, "0 - (nenhum)"); } else { for(int i=0;i<fn;i++){ char line[200]; snprintf(line,sizeof(line),"%d - %s", fbuf[i].id, fbuf[i].nome_fantasia); IupSetStrAttributeId(txtFornecedor, "", i+1, line); } }
 IupSetAttribute(txtFornecedor,"VALUE","1");
 Ihandle *txtFornecedorValor=IupText(NULL);
 /* DatePick para datas */
 Ihandle *txtInicio=IupDatePick(); Ihandle *txtFim=IupDatePick();
 /* data atual em formato aceito pelo DatePick (yyyy/mm/dd) */
 time_t now=time(NULL); struct tm *lt=localtime(&now); char iso_now[32]; snprintf(iso_now,sizeof(iso_now),"%04d/%02d/%02d", (int)(lt->tm_year+1900), (int)(lt->tm_mon+1), (int)lt->tm_mday); IupSetAttribute(txtInicio,"VALUE", iso_now); IupSetAttribute(txtFim,"VALUE", iso_now);
 Ihandle *txtLocal=IupText(NULL);
 ui_set_width_px(txtId, UI_W_ID); ui_set_width_px(txtCliente, UI_W_MED); ui_set_width_px(txtRecurso, UI_W_MED); ui_set_width_px(txtRecursoQtd, UI_W_SHORT); ui_set_width_px(txtEquipe, UI_W_MED); ui_set_width_px(txtEquipeHoras, UI_W_SHORT); ui_set_width_px(txtFornecedor, UI_W_MED); ui_set_width_px(txtFornecedorValor, UI_W_MED); ui_set_width_px(txtInicio, UI_W_MED); ui_set_width_px(txtFim, UI_W_MED); ui_set_width_px(txtLocal, UI_W_XLONG);
 Ihandle *txtFiltroInicio=IupDatePick(); Ihandle *txtFiltroFim=IupDatePick();
 /* iniciar filtros de data vazios para não filtrar por padrão */
 IupSetAttribute(txtFiltroInicio,"MASKNOEMPTY","NO"); IupSetAttribute(txtFiltroFim,"MASKNOEMPTY","NO");
 IupSetAttribute(txtFiltroInicio,"VALUE",""); IupSetAttribute(txtFiltroFim,"VALUE","");
 ui_set_width_px(txtFiltroInicio, UI_W_MED); ui_set_width_px(txtFiltroFim, UI_W_MED);
 /* filtros de Status e Cliente removidos por solicitação */
 Ihandle *btnNovo=IupButton("Novo",NULL); Ihandle *btnSalvar=IupButton("Salvar",NULL); Ihandle *btnAprovar=IupButton("Aprovar",NULL); Ihandle *btnFinalizar=IupButton("Finalizar",NULL); Ihandle *btnExcluir=IupButton("Excluir",NULL); Ihandle *btnAtualizar=IupButton("Filtrar/Atualizar",NULL); Ihandle *btnExportar=IupButton("Exportar CSV",NULL);
 mat_evento=IupMatrix(NULL); IupSetAttribute(mat_evento,"NUMCOL","10"); IupSetAttribute(mat_evento,"NUMLIN","1"); IupSetAttribute(mat_evento,"EXPAND","YES"); IupSetAttribute(mat_evento,"WIDTH1","40"); IupSetAttribute(mat_evento,"WIDTH2","140"); IupSetAttribute(mat_evento,"WIDTH3","160"); IupSetAttribute(mat_evento,"WIDTH4","140"); IupSetAttribute(mat_evento,"WIDTH5","160"); IupSetAttribute(mat_evento,"WIDTH6","80"); IupSetAttribute(mat_evento,"WIDTH7","80"); IupSetAttribute(mat_evento,"WIDTH8","50"); IupSetAttribute(mat_evento,"WIDTH9","80"); IupSetAttribute(mat_evento,"WIDTH10","80");
 Ihandle *rows=IupVbox(
  ui_pair("Código:", txtId),
  ui_pair("Cliente:", txtCliente),
  ui_pair("Recurso ID:", txtRecurso),
  ui_pair("Qtd Recurso:", txtRecursoQtd),
  ui_pair("Equipe ID:", txtEquipe),
  ui_pair("Horas Equipe:", txtEquipeHoras),
  ui_pair("Fornecedor ID:", txtFornecedor),
  ui_pair("Valor Serv.", txtFornecedorValor),
  ui_pair("Data Início:", txtInicio),
  ui_pair("Data Fim:", txtFim),
  ui_pair("Local:", txtLocal),
  NULL);
 ui_style_form(rows);
 Ihandle *filt_row=IupHbox(ui_pair("Filtro Início:", txtFiltroInicio), ui_pair("Filtro Fim:", txtFiltroFim), NULL);
 ui_style_form(filt_row);
 Ihandle *btn_row=IupHbox(IupFill(), btnNovo, btnSalvar, btnAprovar, btnFinalizar, btnExcluir, btnAtualizar, btnExportar, IupFill(), NULL);
 Ihandle *form=IupVbox(IupLabel("Orçamentos e Gestão de Eventos"), rows, filt_row, btn_row, IupSetAttributes(IupFrame(mat_evento), "TITLE=Lista de Eventos"), NULL); ui_style_form(form);
 /* atributos para callbacks (usar handles) */
 const char* names[]={"txtId","txtCliente","txtRecurso","txtRecursoQtd","txtEquipe","txtEquipeHoras","txtFornecedor","txtFornecedorValor","txtInicio","txtFim","txtLocal","txtFiltroInicio","txtFiltroFim"}; Ihandle* widgets[]={txtId,txtCliente,txtRecurso,txtRecursoQtd,txtEquipe,txtEquipeHoras,txtFornecedor,txtFornecedorValor,txtInicio,txtFim,txtLocal,txtFiltroInicio,txtFiltroFim};
 for(int i=0;i<13;i++){
   IupSetAttributeHandle(btnNovo,names[i],widgets[i]);
   IupSetAttributeHandle(btnSalvar,names[i],widgets[i]);
   IupSetAttributeHandle(btnAprovar,names[i],widgets[i]);
   IupSetAttributeHandle(btnFinalizar,names[i],widgets[i]);
   IupSetAttributeHandle(btnExcluir,names[i],widgets[i]);
   IupSetAttributeHandle(btnAtualizar,names[i],widgets[i]);
   IupSetAttributeHandle(btnExportar,names[i],widgets[i]);
   /* também anexa ao matrix para o evento de clique */
   IupSetAttributeHandle(mat_evento,names[i],widgets[i]);
 }
 IupSetCallback(btnNovo, "ACTION", (Icallback)evento_novo_cb); IupSetCallback(btnSalvar, "ACTION", (Icallback)evento_salvar_cb); IupSetCallback(btnAprovar, "ACTION", (Icallback)evento_aprovar_cb); IupSetCallback(btnFinalizar, "ACTION", (Icallback)evento_finalizar_cb); IupSetCallback(btnExcluir, "ACTION", (Icallback)evento_excluir_cb); IupSetCallback(btnAtualizar, "ACTION", (Icallback)evento_atualizar_cb); IupSetCallback(btnExportar, "ACTION", (Icallback)evento_exportar_cb); IupSetCallback(mat_evento, "CLICK_CB", (Icallback)evento_click_cb);
 evento_view_recarregar(mat_evento);
 return form; }

void evento_view_mostrar(void){ if(!dlg_evento){ dlg_evento=IupDialog(evento_view_create()); IupSetAttribute(dlg_evento,"TITLE","Eventos"); IupSetAttribute(dlg_evento,"SIZE","960x560"); } IupShowXY(dlg_evento,IUP_CENTER,IUP_CENTER); }
