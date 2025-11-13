#ifndef OPERADOR_CONTROLLER_H
#define OPERADOR_CONTROLLER_H

#include "../model/operador.h"
#include <iup.h>

int operador_salvar(Operador o);
int operador_excluir(int id);
int operador_listar(Operador *buffer, int max);

int operador_salvar_cb(Ihandle *self);
int operador_excluir_cb(Ihandle *self);

#endif