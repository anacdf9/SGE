#ifndef EQUIPE_CONTROLLER_H
#define EQUIPE_CONTROLLER_H

#include "../model/equipe.h"
#include <iup.h>

int equipe_salvar(Equipe e);
int equipe_excluir(int id);
int equipe_listar(Equipe *buffer, int max);

int equipe_salvar_cb(Ihandle *self);
int equipe_excluir_cb(Ihandle *self);

#endif