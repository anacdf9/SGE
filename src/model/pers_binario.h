#ifndef PERS_BINARIO_H
#define PERS_BINARIO_H

#include "cliente.h"

void pers_binario_inicializar(void);
void pers_binario_finalizar(void);

int pers_binario_salvar_cliente(Cliente c);
int pers_binario_carregar_clientes(Cliente *lista, int max);
int pers_binario_remover_cliente(int id);

#endif // PERS_BINARIO_H
