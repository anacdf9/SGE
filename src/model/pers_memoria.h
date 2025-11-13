#ifndef PERS_MEMORIA_H
#define PERS_MEMORIA_H

#include "cliente.h"

void pers_memoria_inicializar(void);
void pers_memoria_finalizar(void);

int pers_memoria_salvar_cliente(Cliente c);
int pers_memoria_carregar_clientes(Cliente *lista, int max);
int pers_memoria_remover_cliente(int id);

#endif // PERS_MEMORIA_H