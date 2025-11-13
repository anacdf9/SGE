#ifndef CLIENTE_CONTROLLER_H
#define CLIENTE_CONTROLLER_H

#include "../model/cliente.h"
#include <iup.h>

/* Controladores diretos (ligam model e view) */
int cliente_salvar(Cliente c);
int cliente_excluir(int id);
int cliente_listar(Cliente *buffer, int max);

/* Callbacks da interface */
int cliente_salvar_cb(Ihandle *self);
int cliente_excluir_cb(Ihandle *self);
int cliente_editar_cb(Ihandle *self);

#endif
