#ifndef EVENTO_CONTROLLER_H
#define EVENTO_CONTROLLER_H

#include "../model/evento.h"

int evento_salvar(Evento e);
int evento_aprovar(int id);
int evento_finalizar(int id);
int evento_listar(Evento *buffer,int max);
int evento_excluir(int id);
int evento_recalcular_totais(int id);

const char* evento_status_para_str(EventoStatus status);

#endif
