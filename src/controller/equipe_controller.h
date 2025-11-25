#ifndef EQUIPE_CONTROLLER_H
#define EQUIPE_CONTROLLER_H

#include <iup.h>
#include "../model/equipe.h"

int equipe_salvar(Equipe e);
int equipe_listar(Equipe *lista, int max);
int equipe_excluir(int id);
int equipe_obter(int id, Equipe *out); // <-- Adicionado

// callbacks usados pela view
int equipe_salvar_cb(Ihandle *self);
int equipe_excluir_cb(Ihandle *self);

#endif