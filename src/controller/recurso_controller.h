#ifndef RECURSO_CONTROLLER_H
#define RECURSO_CONTROLLER_H

#include <iup.h>
#include "../model/recurso.h"

int recurso_salvar(Recurso r);
int recurso_listar(Recurso *lista, int max);
int recurso_excluir(int id);
int recurso_obter(int id, Recurso *out); 

// callbacks usados pela view
int recurso_salvar_cb(Ihandle *self);
int recurso_excluir_cb(Ihandle *self);

#endif
