#ifndef EVENTO_ITEM_H
#define EVENTO_ITEM_H

typedef struct {
    int id;          /* identificador do item */
    int evento_id;   /* FK para Evento.id */
    int recurso_id;  /* recurso/equipamento da produtora */
    int quantidade;  /* quantidade desse recurso para o evento */
} EventoItem;

#endif
