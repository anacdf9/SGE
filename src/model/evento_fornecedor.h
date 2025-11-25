#ifndef EVENTO_FORNECEDOR_H
#define EVENTO_FORNECEDOR_H

// Representa a associação de um fornecedor/parceiro a um evento.
typedef struct {
    int id;             // ID único da associação
    int evento_id;      // ID do evento
    int fornecedor_id;  // ID do fornecedor
    double valor_servico; // Custo do serviço contratado do fornecedor
} EventoFornecedor;

#endif
