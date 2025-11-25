#ifndef EVENTO_H
#define EVENTO_H

typedef enum {
	EVENTO_STATUS_ORCAMENTO = 1,
	EVENTO_STATUS_APROVADO  = 2,
	EVENTO_STATUS_FINALIZADO = 3
} EventoStatus;

typedef enum {
    EVENTO_TIPO_ORCAMENTO      = 1,
    EVENTO_TIPO_ORDEM_SERVICO = 2
} EventoTipo;

typedef struct {
	int id;
	char nome[120];
	int cliente_id;
	int operador_id;
	int produtora_id;
	char data_inicio[11]; /* YYYY-MM-DD */
	char data_fim[11];    /* YYYY-MM-DD */
	char local[120];
	EventoTipo tipo;
	EventoStatus status;
	double total_estimado;
	double total_final;
} Evento;

#endif
