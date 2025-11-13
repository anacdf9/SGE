#ifndef PRODUTORA_CONTROLLER_H
#define PRODUTORA_CONTROLLER_H

#include "../model/produtora.h"
#include <iup.h>

int produtora_salvar(Produtora p);
int produtora_obter(Produtora *out);

int produtora_salvar_cb(Ihandle *self);
int produtora_carregar_cb(Ihandle *self);

#endif