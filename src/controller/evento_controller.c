#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "evento_controller.h"
#include "../model/pers.h"
#include "../model/cliente.h"
#include "../model/recurso.h"
#include "../model/equipe.h"
#include "../model/fornecedor.h"

static int evento_next_id(void){ Evento buf[1024]; int n=pers_carregar_eventos(buf,1024); int m=0; for(int i=0;i<n;i++) if(buf[i].id>m) m=buf[i].id; return m+1; }

int evento_listar(Evento *buffer,int max){ return pers_carregar_eventos(buffer,max); }
int evento_excluir(int id){ return pers_remover_evento(id); }

static int parse_date(const char *s){ if(!s||strlen(s)<10) return 0; int y,m,d; if(sscanf(s,"%d-%d-%d",&y,&m,&d)!=3) return 0; return y*10000 + m*100 + d; }
static int dias_intervalo(const char *ini,const char *fim){ int a=parse_date(ini); int b=parse_date(fim); if(a==0||b==0||b<a) return 0; int ay=a/10000, am=(a/100)%100, ad=a%100; int by=b/10000, bm=(b/100)%100, bd=b%100; /* simplificado: ignora meses diferentes com cálculo exato */ int diff = bd - ad + 1; if(am!=bm || ay!=by) diff = 1; if(diff<1) diff=1; return diff; }

static int evento_conflito(Evento novo){
	Evento buf[1024]; int n=pers_carregar_eventos(buf,1024);
	int ini=parse_date(novo.data_inicio); int fim=parse_date(novo.data_fim);
	if(ini==0||fim==0||fim<ini) return 1; /* datas inválidas tratadas como conflito */
	/* Se não houver recurso selecionado, não considerar conflito de estoque */
	if(novo.recurso_id<=0) return 0;
	/* Verifica disponibilidade por quantidade do recurso */
	Recurso rbuf[256]; int rn=pers_carregar_recursos(rbuf,256); int recurso_qtd_total=0; int recurso_found=0;
	for(int i=0;i<rn;i++){ if(rbuf[i].id==novo.recurso_id){ recurso_qtd_total=rbuf[i].quantidade; recurso_found=1; break; } }
	if(!recurso_found) return 0; /* recurso não encontrado: não bloquear aprovação */
	int qtd_alocada_intervalo=0;
	for(int i=0;i<n;i++){
		if(buf[i].status!=EVENTO_STATUS_APROVADO) continue;
		if(buf[i].recurso_id!=novo.recurso_id) continue;
		int oini=parse_date(buf[i].data_inicio); int ofim=parse_date(buf[i].data_fim);
		if(!(fim < oini || ini > ofim)){
			qtd_alocada_intervalo += buf[i].recurso_qtd;
		}
	}
	if(qtd_alocada_intervalo + novo.recurso_qtd > recurso_qtd_total) return 1; /* ultrapassa estoque */
	return 0;
}

static double calcular_estimado(Evento e){ Recurso rbuf[256]; int rn=pers_carregar_recursos(rbuf,256); double recurso_custo=0; for(int i=0;i<rn;i++) if(rbuf[i].id==e.recurso_id){ int dias=dias_intervalo(e.data_inicio,e.data_fim); recurso_custo = rbuf[i].valor_locacao * e.recurso_qtd * dias; break; }
 Equipe eb[256]; int en=pers_carregar_equipes(eb,256); double equipe_custo=0; for(int i=0;i<en;i++) if(eb[i].id==e.equipe_id){ equipe_custo = eb[i].valor_hora * e.equipe_horas; break; }
 double fornecedor_custo = e.fornecedor_valor; return recurso_custo + equipe_custo + fornecedor_custo; }

int evento_salvar(Evento e){ if(e.id<=0){ e.id=evento_next_id(); if(e.id<=0) e.id=1; } if(e.status==EVENTO_STATUS_RASCUNHO){ e.total_estimado = calcular_estimado(e); } return pers_salvar_evento(e); }

int evento_aprovar(int id){ Evento buf[1024]; int n=pers_carregar_eventos(buf,1024); for(int i=0;i<n;i++){ if(buf[i].id==id){ if(buf[i].status!=EVENTO_STATUS_RASCUNHO) return 0; if(evento_conflito(buf[i])) return 0; buf[i].status=EVENTO_STATUS_APROVADO; buf[i].total_estimado=calcular_estimado(buf[i]); return pers_salvar_evento(buf[i]); } } return 0; }

int evento_finalizar(int id){ Evento buf[1024]; int n=pers_carregar_eventos(buf,1024); for(int i=0;i<n;i++){ if(buf[i].id==id){ if(buf[i].status!=EVENTO_STATUS_APROVADO) return 0; buf[i].status=EVENTO_STATUS_FINALIZADO; buf[i].total_final=buf[i].total_estimado; return pers_salvar_evento(buf[i]); } } return 0; }
