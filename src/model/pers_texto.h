#ifndef PERS_TEXTO_H
#define PERS_TEXTO_H

#include "cliente.h"

void pers_texto_inicializar(void);
void pers_texto_finalizar(void);

int pers_texto_salvar_cliente(Cliente c);
int pers_texto_carregar_clientes(Cliente *lista, int max);
int pers_texto_remover_cliente(int id);

#endif // PERS_TEXTO_H