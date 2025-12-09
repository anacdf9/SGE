#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "feedback_controller.h"
#include "../model/pers.h"

 

int feedback_comparar_datas(const char *data1, const char *data2) {
    if (strcmp(data1, data2) < 0) return -1;
    if (strcmp(data1, data2) > 0) return 1;
    return 0;
}

int feedback_data_no_intervalo(const char *data, const char *min, const char *max) {
    if (strlen(min) > 0 && feedback_comparar_datas(data, min) < 0) return 0;
    if (strlen(max) > 0 && feedback_comparar_datas(data, max) > 0) return 0;
    return 1;
}

// Auxiliar para comparar strings (para qsort)
static int comparar_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

/* ========================================================================
 * RELATÓRIOS DE CLIENTES
 * ======================================================================== */

int feedback_relatorio_clientes(Cliente *buffer, int max, FiltroCliente filtro) {
    if (!buffer || max <= 0) return 0;
    
    Cliente lista_temp[1024];
    int total = pers_carregar_clientes(lista_temp, 1024);
    int count = 0;
    
    // Aplicar filtros
    for (int i = 0; i < total && count < max; i++) {
        // Filtro: faixa de códigos
        if (lista_temp[i].id < filtro.codigo_min || lista_temp[i].id > filtro.codigo_max) {
            continue;
        }
        buffer[count++] = lista_temp[i];
    }
    
    // Ordenação: alfabética ou por código
    if (filtro.ordem_alfabetica == 1) {
        // Simples ordenação por nome usando qsort
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (strcmp(buffer[i].nome, buffer[j].nome) > 0) {
                    Cliente temp = buffer[i];
                    buffer[i] = buffer[j];
                    buffer[j] = temp;
                }
            }
        }
    } else {
        // Ordenar por código 
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (buffer[i].id > buffer[j].id) {
                    Cliente temp = buffer[i];
                    buffer[i] = buffer[j];
                    buffer[j] = temp;
                }
            }
        }
    }
    
    return count;
}

int feedback_exportar_clientes_csv(const char *caminho, FiltroCliente filtro) {
    if (!caminho) return 0;
    
    FILE *fp = fopen(caminho, "w");
    if (!fp) return 0;
    
    // Cabeçalho
    fprintf(fp, "Código;Nome;Endereço;CPF/CNPJ;Telefone;Email;Contato\n");
    
    Cliente buffer[1024];
    int total = feedback_relatorio_clientes(buffer, 1024, filtro);
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "%d;%s;%s;%s;%s;%s;%s\n",
            buffer[i].id,
            buffer[i].nome,
            buffer[i].endereco,
            buffer[i].cpf_cnpj,
            buffer[i].telefone,
            buffer[i].email,
            buffer[i].contato);
    }
    
    fclose(fp);
    return 1;
}

/* ========================================================================
 *  EVENTOS
 * ======================================================================== */

void feedback_obter_cliente_nome(int cliente_id, char *buffer, int max) {
    if (!buffer || max <= 0) return;
    strcpy(buffer, "Desconhecido");
    
    Cliente clientes[1024];
    int total = pers_carregar_clientes(clientes, 1024);
    
    for (int i = 0; i < total; i++) {
        if (clientes[i].id == cliente_id) {
            strncpy(buffer, clientes[i].nome, max - 1);
            buffer[max - 1] = '\0';
            return;
        }
    }
}

int feedback_relatorio_eventos(Evento *buffer, int max, FiltroEvento filtro) {
    if (!buffer || max <= 0) return 0;
    
    Evento lista_temp[1024];
    int total = pers_carregar_eventos(lista_temp, 1024);
    int count = 0;
    
    for (int i = 0; i < total && count < max; i++) {
        // Filtro: cliente
        if (filtro.cliente_id >= 0 && lista_temp[i].cliente_id != filtro.cliente_id) {
            continue;
        }
        
        // Filtro: período de data
        if (!feedback_data_no_intervalo(lista_temp[i].data_inicio, filtro.data_min, filtro.data_max)) {
            continue;
        }
        
        // Filtro: status
        if (filtro.status > 0 && lista_temp[i].status != filtro.status) {
            continue;
        }
        
        buffer[count++] = lista_temp[i];
    }
    
    return count;
}

int feedback_exportar_eventos_csv(const char *caminho, FiltroEvento filtro) {
    if (!caminho) return 0;
    
    FILE *fp = fopen(caminho, "w");
    if (!fp) return 0;
    
    fprintf(fp, "Código;Nome;Cliente;Data Início;Data Fim;Local;Tipo;Status;Total Estimado;Total Final\n");
    
    Evento buffer[1024];
    int total = feedback_relatorio_eventos(buffer, 1024, filtro);
    
    for (int i = 0; i < total; i++) {
        char cliente_nome[CLIENTE_NOME_LEN];
        feedback_obter_cliente_nome(buffer[i].cliente_id, cliente_nome, sizeof(cliente_nome));
        
        const char *tipo_str = (buffer[i].tipo == EVENTO_TIPO_ORCAMENTO) ? "Orçamento" : "Ordem de Serviço";
        const char *status_str = "Desconhecido";
        if (buffer[i].status == EVENTO_STATUS_ORCAMENTO) status_str = "Orçamento";
        else if (buffer[i].status == EVENTO_STATUS_APROVADO) status_str = "Aprovado";
        else if (buffer[i].status == EVENTO_STATUS_FINALIZADO) status_str = "Finalizado";
        
        fprintf(fp, "%d;%s;%s;%s;%s;%s;%s;%s;%.2f;%.2f\n",
            buffer[i].id,
            buffer[i].nome,
            cliente_nome,
            buffer[i].data_inicio,
            buffer[i].data_fim,
            buffer[i].local,
            tipo_str,
            status_str,
            buffer[i].total_estimado,
            buffer[i].total_final);
    }
    
    fclose(fp);
    return 1;
}

/* ========================================================================
 *  CRONOGRAMA DE RECURSOS
 * ======================================================================== */

int feedback_cronograma_alocacao(AlocacaoRecurso *buffer, int max, FiltroRecurso filtro) {
    if (!buffer || max <= 0) return 0;
    
    Evento eventos[1024];
    int total_eventos = pers_carregar_eventos(eventos, 1024);
    int count = 0;
    
    Recurso recursos[1024];
    int total_recursos = pers_carregar_recursos(recursos, 1024);
    
    // Iterar eventos para criar registros de alocação
    for (int i = 0; i < total_eventos && count < max; i++) {
        // Filtro: período de data
        if (!feedback_data_no_intervalo(eventos[i].data_inicio, filtro.data_min, filtro.data_max)) {
            continue;
        }
        // Buscar todos os itens de eventos
        EventoItem itens_todos[500];
        int total_itens_todos = pers_carregar_evento_itens(itens_todos, 500);
        
        // Filtrar itens deste evento
        EventoItem itens[500];
        int total_itens = 0;
        for (int x = 0; x < total_itens_todos; x++) {
            if (itens_todos[x].evento_id == eventos[i].id) {
                itens[total_itens++] = itens_todos[x];
            }
        }
        
        for (int j = 0; j < total_itens && count < max; j++) {
            // Filtro: recurso específico
            if (filtro.recurso_id >= 0 && itens[j].recurso_id != filtro.recurso_id) {
                continue;
            }
            
            // Buscar nome do recurso
            char recurso_desc[RECURSO_DESC_LEN] = "Desconhecido";
            for (int k = 0; k < total_recursos; k++) {
                if (recursos[k].id == itens[j].recurso_id) {
                    strcpy(recurso_desc, recursos[k].descricao);
                    break;
                }
            }
            
            buffer[count].recurso_id = itens[j].recurso_id;
            strcpy(buffer[count].recurso_desc, recurso_desc);
            buffer[count].evento_id = eventos[i].id;
            strcpy(buffer[count].evento_nome, eventos[i].nome);
            strcpy(buffer[count].data_inicio, eventos[i].data_inicio);
            strcpy(buffer[count].data_fim, eventos[i].data_fim);
            
            count++;
        }
    }
    
    return count;
}

int feedback_exportar_cronograma_csv(const char *caminho, FiltroRecurso filtro) {
    if (!caminho) return 0;
    
    FILE *fp = fopen(caminho, "w");
    if (!fp) return 0;
    
    fprintf(fp, "Código Recurso;Descrição;Evento;Data Início;Data Fim\n");
    
    AlocacaoRecurso buffer[1024];
    int total = feedback_cronograma_alocacao(buffer, 1024, filtro);
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "%d;%s;%s;%s;%s\n",
            buffer[i].recurso_id,
            buffer[i].recurso_desc,
            buffer[i].evento_nome,
            buffer[i].data_inicio,
            buffer[i].data_fim);
    }
    
    fclose(fp);
    return 1;
}

/* ========================================================================
 * EQUIPAMENTOS
 * ======================================================================== */

int feedback_relatorio_equipamentos(Recurso *buffer, int max, FiltroEquipamento filtro) {
    if (!buffer || max <= 0) return 0;
    
    Recurso lista_temp[1024];
    int total = pers_carregar_recursos(lista_temp, 1024);
    int count = 0;
    
    for (int i = 0; i < total && count < max; i++) {
        // Filtro: faixa de códigos
        if (lista_temp[i].id < filtro.codigo_min || lista_temp[i].id > filtro.codigo_max) {
            continue;
        }
        
        // Filtro: categoria
        if (strlen(filtro.categoria) > 0 && strcmp(lista_temp[i].categoria, filtro.categoria) != 0) {
            continue;
        }
        
        buffer[count++] = lista_temp[i];
    }
    
    return count;
}

int feedback_exportar_equipamentos_csv(const char *caminho, FiltroEquipamento filtro) {
    if (!caminho) return 0;
    
    FILE *fp = fopen(caminho, "w");
    if (!fp) return 0;
    
    fprintf(fp, "Código;Descrição;Categoria;Quantidade;Preço de Custo;Valor de Locação\n");
    
    Recurso buffer[1024];
    int total = feedback_relatorio_equipamentos(buffer, 1024, filtro);
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "%d;%s;%s;%d;%.2f;%.2f\n",
            buffer[i].id,
            buffer[i].descricao,
            buffer[i].categoria,
            buffer[i].quantidade,
            buffer[i].preco_custo,
            buffer[i].valor_locacao);
    }
    
    fclose(fp);
    return 1;
}

/* ========================================================================
 * CONTAS A RECEBER
 * ======================================================================== */

int feedback_relatorio_contas_receber(ContaReceber *buffer, int max, FiltroContasReceber filtro) {
    if (!buffer || max <= 0) return 0;
    
    ContaReceber lista_temp[1024];
    int total = pers_carregar_contas_receber(lista_temp, 1024);
    int count = 0;
    
    Evento eventos[1024];
    int total_eventos = pers_carregar_eventos(eventos, 1024);
    
    for (int i = 0; i < total && count < max; i++) {
        // Filtro: cliente
        if (filtro.cliente_id >= 0) {
            int encontrou = 0;
            for (int j = 0; j < total_eventos; j++) {
                if (eventos[j].id == lista_temp[i].evento_id && 
                    eventos[j].cliente_id == filtro.cliente_id) {
                    encontrou = 1;
                    break;
                }
            }
            if (!encontrou) continue;
        }
        
        // Filtro: período de data de vencimento
        if (!feedback_data_no_intervalo(lista_temp[i].data_vencimento, filtro.data_min, filtro.data_max)) {
            continue;
        }
        
        buffer[count++] = lista_temp[i];
    }
    
    return count;
}

int feedback_exportar_contas_receber_csv(const char *caminho, FiltroContasReceber filtro) {
    if (!caminho) return 0;
    
    FILE *fp = fopen(caminho, "w");
    if (!fp) return 0;
    
    fprintf(fp, "Código;Evento;Data Emissão;Data Vencimento;Valor Fatura;Valor Recebido;Status\n");
    
    ContaReceber buffer[1024];
    int total = feedback_relatorio_contas_receber(buffer, 1024, filtro);
    
    for (int i = 0; i < total; i++) {
        const char *status_str = "Aberto";
        if (buffer[i].status == 1) status_str = "Parcial";
        else if (buffer[i].status == 2) status_str = "Liquidado";
        
        fprintf(fp, "%d;%d;%s;%s;%.2f;%.2f;%s\n",
            buffer[i].id,
            buffer[i].evento_id,
            buffer[i].data_emissao,
            buffer[i].data_vencimento,
            buffer[i].valor_fatura,
            buffer[i].valor_recebido,
            status_str);
    }
    
    fclose(fp);
    return 1;
}

/* ========================================================================
 *  CONTAS A PAGAR
 * ======================================================================== */

void feedback_obter_fornecedor_nome(int fornecedor_id, char *buffer, int max) {
    if (!buffer || max <= 0) return;
    strcpy(buffer, "Desconhecido");
    
    Fornecedor fornecedores[1024];
    int total = pers_carregar_fornecedores(fornecedores, 1024);
    
    for (int i = 0; i < total; i++) {
        if (fornecedores[i].id == fornecedor_id) {
            strncpy(buffer, fornecedores[i].nome_fantasia, max - 1);
            buffer[max - 1] = '\0';
            return;
        }
    }
}

int feedback_relatorio_contas_pagar(ContaPagar *buffer, int max, FiltroContasPagar filtro) {
    if (!buffer || max <= 0) return 0;
    
    ContaPagar lista_temp[1024];
    int total = pers_carregar_contas_pagar(lista_temp, 1024);
    int count = 0;
    
    Compra compras[1024];
    int total_compras = pers_carregar_compras(compras, 1024);
    
    for (int i = 0; i < total && count < max; i++) {
        // Filtro: fornecedor
        if (filtro.fornecedor_id >= 0) {
            int encontrou = 0;
            for (int j = 0; j < total_compras; j++) {
                if (compras[j].id == lista_temp[i].compra_id &&
                    compras[j].fornecedor_id == filtro.fornecedor_id) {
                    encontrou = 1;
                    break;
                }
            }
            if (!encontrou) continue;
        }
        
        // Filtro: período de data de vencimento
        if (!feedback_data_no_intervalo(lista_temp[i].data_vencimento, filtro.data_min, filtro.data_max)) {
            continue;
        }
        
        buffer[count++] = lista_temp[i];
    }
    
    return count;
}

int feedback_exportar_contas_pagar_csv(const char *caminho, FiltroContasPagar filtro) {
    if (!caminho) return 0;
    
    FILE *fp = fopen(caminho, "w");
    if (!fp) return 0;
    
    fprintf(fp, "Código;Fornecedor;Compra;Data Vencimento;Valor;Status\n");
    
    ContaPagar buffer[1024];
    int total = feedback_relatorio_contas_pagar(buffer, 1024, filtro);
    
    for (int i = 0; i < total; i++) {
        char fornecedor_nome[FORN_NOME_FANTASIA_LEN];
        feedback_obter_fornecedor_nome(buffer[i].fornecedor_id, fornecedor_nome, sizeof(fornecedor_nome));
        
        const char *status_str = (buffer[i].status == 0) ? "Aberto" : "Pago";
        
        fprintf(fp, "%d;%s;%d;%s;%.2f;%s\n",
            buffer[i].id,
            fornecedor_nome,
            buffer[i].compra_id,
            buffer[i].data_vencimento,
            buffer[i].valor,
            status_str);
    }
    
    fclose(fp);
    return 1;
}

/* ========================================================================
 * MOVIMENTAÇÃO DE CAIXA
 * ======================================================================== */

int feedback_relatorio_caixa(CaixaLancamento *buffer, int max, FiltroCaixa filtro) {
    if (!buffer || max <= 0) return 0;
    
    CaixaLancamento lista_temp[1024];
    int total = pers_carregar_caixa(lista_temp, 1024);
    int count = 0;
    
    for (int i = 0; i < total && count < max; i++) {
        // Filtro: período de data
        if (!feedback_data_no_intervalo(lista_temp[i].data, filtro.data_min, filtro.data_max)) {
            continue;
        }
        
        buffer[count++] = lista_temp[i];
    }
    
    return count;
}

int feedback_exportar_caixa_csv(const char *caminho, FiltroCaixa filtro) {
    if (!caminho) return 0;
    
    FILE *fp = fopen(caminho, "w");
    if (!fp) return 0;
    
    fprintf(fp, "Código;Data;Tipo;Forma;Valor;Descrição\n");
    
    CaixaLancamento buffer[1024];
    int total = feedback_relatorio_caixa(buffer, 1024, filtro);
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "%d;%s;%s;%s;%.2f;%s\n",
            buffer[i].id,
            buffer[i].data,
            buffer[i].tipo,
            buffer[i].forma,
            buffer[i].valor,
            buffer[i].descricao);
    }
    
    fclose(fp);
    return 1;
}
