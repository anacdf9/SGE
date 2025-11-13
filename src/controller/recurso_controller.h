#ifndef RECURSO_CONTROLLER_H
#define RECURSO_CONTROLLER_H

#include "../model/recurso.h"
#include <iup.h>

int recurso_salvar(Recurso r);
int recurso_excluir(int id);
int recurso_listar(Recurso *buffer, int max);

int recurso_salvar_cb(Ihandle *self);
int recurso_excluir_cb(Ihandle *self);

#endif
