#ifndef PERS_H
#define PERS_H

#include "cliente.h"
#include "config.h"
#include <stdlib.h>   

/* Inicializa o sistema de persistência (usa TIPO_PERSISTENCIA por padrão) */
void pers_inicializar(TipoPersistencia tipo);
void pers_finalizar(void);

/* CRUD cliente (fachada) */
int pers_salvar_cliente(Cliente c);
int pers_carregar_clientes(Cliente *lista, int max);
int pers_remover_cliente(int id);

#endif // PERS_H