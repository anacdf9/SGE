#ifndef FORNECEDOR_CONTROLLER_H
#define FORNECEDOR_CONTROLLER_H

#include "../model/fornecedor.h"
#include <iup.h>

int fornecedor_salvar(Fornecedor f);
int fornecedor_excluir(int id);
int fornecedor_listar(Fornecedor *buffer, int max);

int fornecedor_salvar_cb(Ihandle *self);
int fornecedor_excluir_cb(Ihandle *self);

#endif