#ifndef EVENTO_EQUIPE_H
#define EVENTO_EQUIPE_H

// Representa a associação de uma equipe a um evento.
typedef struct {
    int id;          // ID único da associação
    int evento_id;   // ID do evento
    int equipe_id;   // ID da equipe
    double horas_trabalhadas; // Horas planejadas para a equipe neste evento
} EventoEquipe;

#endif
