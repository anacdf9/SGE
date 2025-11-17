#ifndef EVENTO_H
#define EVENTO_H

typedef enum {
	EVENTO_STATUS_RASCUNHO = 0,
	EVENTO_STATUS_APROVADO = 1,
	EVENTO_STATUS_FINALIZADO = 2
} EventoStatus;

typedef struct {
	int id;
	int cliente_id;
	int recurso_id;
	int recurso_qtd;
	int equipe_id;
	double equipe_horas;
	int fornecedor_id;
	double fornecedor_valor;
	char data_inicio[11]; /* YYYY-MM-DD */
	char data_fim[11];    /* YYYY-MM-DD */
	char local[120];
	EventoStatus status;
	double total_estimado;
	double total_final;
} Evento;

#endif
