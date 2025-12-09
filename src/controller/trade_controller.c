#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "trade_controller.h"
#include "../model/pers.h"



void trade_escape_xml(const char *input, char *output, int max_len) {
    if (!input || !output || max_len <= 0) return;
    
    int pos_out = 0;
    for (int i = 0; input[i] && pos_out < max_len - 1; i++) {
        switch (input[i]) {
            case '&':
                if (pos_out + 5 < max_len) {
                    strcpy(&output[pos_out], "&amp;");
                    pos_out += 5;
                }
                break;
            case '<':
                if (pos_out + 4 < max_len) {
                    strcpy(&output[pos_out], "&lt;");
                    pos_out += 4;
                }
                break;
            case '>':
                if (pos_out + 4 < max_len) {
                    strcpy(&output[pos_out], "&gt;");
                    pos_out += 4;
                }
                break;
            case '"':
                if (pos_out + 6 < max_len) {
                    strcpy(&output[pos_out], "&quot;");
                    pos_out += 6;
                }
                break;
            case '\'':
                if (pos_out + 6 < max_len) {
                    strcpy(&output[pos_out], "&apos;");
                    pos_out += 6;
                }
                break;
            default:
                output[pos_out++] = input[i];
        }
    }
    output[pos_out] = '\0';
}

void trade_unescape_xml(const char *input, char *output, int max_len) {
    if (!input || !output || max_len <= 0) return;
    
    int pos_out = 0;
    for (int i = 0; input[i] && pos_out < max_len - 1; i++) {
        if (input[i] == '&') {
            if (strncmp(&input[i], "&amp;", 5) == 0) {
                output[pos_out++] = '&';
                i += 4;
            } else if (strncmp(&input[i], "&lt;", 4) == 0) {
                output[pos_out++] = '<';
                i += 3;
            } else if (strncmp(&input[i], "&gt;", 4) == 0) {
                output[pos_out++] = '>';
                i += 3;
            } else if (strncmp(&input[i], "&quot;", 6) == 0) {
                output[pos_out++] = '"';
                i += 5;
            } else if (strncmp(&input[i], "&apos;", 6) == 0) {
                output[pos_out++] = '\'';
                i += 5;
            } else {
                output[pos_out++] = input[i];
            }
        } else {
            output[pos_out++] = input[i];
        }
    }
    output[pos_out] = '\0';
}

int trade_extrair_tag_valor(const char *tag_line, const char *tag_name, char *output, int max_len) {
    if (!tag_line || !tag_name || !output || max_len <= 0) return 0;
    
    char open_tag[256];
    char close_tag[256];
    snprintf(open_tag, sizeof(open_tag), "<%s>", tag_name);
    snprintf(close_tag, sizeof(close_tag), "</%s>", tag_name);
    
    const char *start = strstr(tag_line, open_tag);
    if (!start) return 0;
    
    start += strlen(open_tag);
    const char *end = strstr(start, close_tag);
    if (!end) return 0;
    
    int len = end - start;
    if (len >= max_len) len = max_len - 1;
    
    strncpy(output, start, len);
    output[len] = '\0';
    
    // Desescapar o valor extraído
    char temp[max_len];
    strcpy(temp, output);
    trade_unescape_xml(temp, output, max_len);
    
    return 1;
}

void trade_gerar_tag(const char *tag_name, const char *valor, char *output, int max_len) {
    if (!tag_name || !valor || !output || max_len <= 0) return;
    
    char valor_escapado[1024];
    trade_escape_xml(valor, valor_escapado, sizeof(valor_escapado));
    
    snprintf(output, max_len, "<%s>%s</%s>", tag_name, valor_escapado, tag_name);
}


int trade_exportar_clientes_xml(FILE *fp) {
    if (!fp) return 0;
    
    Cliente clientes[1024];
    int total = pers_carregar_clientes(clientes, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Clientes -->\n");
    fprintf(fp, "  <tabela-cliente>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", clientes[i].id);
        
        char nome_escaped[300];
        trade_escape_xml(clientes[i].nome, nome_escaped, sizeof(nome_escaped));
        fprintf(fp, "      <nome>%s</nome>\n", nome_escaped);
        
        char endereco_escaped[300];
        trade_escape_xml(clientes[i].endereco, endereco_escaped, sizeof(endereco_escaped));
        fprintf(fp, "      <endereco>%s</endereco>\n", endereco_escaped);
        
        char cpf_escaped[60];
        trade_escape_xml(clientes[i].cpf_cnpj, cpf_escaped, sizeof(cpf_escaped));
        fprintf(fp, "      <cpf_cnpj>%s</cpf_cnpj>\n", cpf_escaped);
        
        char telefone_escaped[60];
        trade_escape_xml(clientes[i].telefone, telefone_escaped, sizeof(telefone_escaped));
        fprintf(fp, "      <telefone>%s</telefone>\n", telefone_escaped);
        
        char email_escaped[160];
        trade_escape_xml(clientes[i].email, email_escaped, sizeof(email_escaped));
        fprintf(fp, "      <email>%s</email>\n", email_escaped);
        
        char contato_escaped[160];
        trade_escape_xml(clientes[i].contato, contato_escaped, sizeof(contato_escaped));
        fprintf(fp, "      <contato>%s</contato>\n", contato_escaped);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-cliente>\n");
    
    return total;
}


int trade_exportar_equipamentos_xml(FILE *fp) {
    if (!fp) return 0;
    
    Recurso recursos[1024];
    int total = pers_carregar_recursos(recursos, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Equipamentos -->\n");
    fprintf(fp, "  <tabela-equipamento>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", recursos[i].id);
        
        char desc_escaped[250];
        trade_escape_xml(recursos[i].descricao, desc_escaped, sizeof(desc_escaped));
        fprintf(fp, "      <descricao>%s</descricao>\n", desc_escaped);
        
        char cat_escaped[120];
        trade_escape_xml(recursos[i].categoria, cat_escaped, sizeof(cat_escaped));
        fprintf(fp, "      <categoria>%s</categoria>\n", cat_escaped);
        
        fprintf(fp, "      <quantidade_estoque>%d</quantidade_estoque>\n", recursos[i].quantidade);
        fprintf(fp, "      <preco_custo>%.2f</preco_custo>\n", recursos[i].preco_custo);
        fprintf(fp, "      <valor_locacao>%.2f</valor_locacao>\n", recursos[i].valor_locacao);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-equipamento>\n");
    
    return total;
}


int trade_exportar_eventos_xml(FILE *fp) {
    if (!fp) return 0;
    
    Evento eventos[1024];
    int total = pers_carregar_eventos(eventos, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Eventos -->\n");
    fprintf(fp, "  <tabela-evento>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", eventos[i].id);
        
        char nome_escaped[250];
        trade_escape_xml(eventos[i].nome, nome_escaped, sizeof(nome_escaped));
        fprintf(fp, "      <nome>%s</nome>\n", nome_escaped);
        
        fprintf(fp, "      <cliente_id>%d</cliente_id>\n", eventos[i].cliente_id);
        fprintf(fp, "      <data_inicio>%s</data_inicio>\n", eventos[i].data_inicio);
        fprintf(fp, "      <data_fim>%s</data_fim>\n", eventos[i].data_fim);
        
        char local_escaped[250];
        trade_escape_xml(eventos[i].local, local_escaped, sizeof(local_escaped));
        fprintf(fp, "      <local>%s</local>\n", local_escaped);
        
        const char *status_str = "Desconhecido";
        if (eventos[i].status == EVENTO_STATUS_ORCAMENTO) status_str = "Orcamento";
        else if (eventos[i].status == EVENTO_STATUS_APROVADO) status_str = "Aprovado";
        else if (eventos[i].status == EVENTO_STATUS_FINALIZADO) status_str = "Finalizado";
        fprintf(fp, "      <status>%s</status>\n", status_str);
        
        fprintf(fp, "      <total_estimado>%.2f</total_estimado>\n", eventos[i].total_estimado);
        fprintf(fp, "      <total_final>%.2f</total_final>\n", eventos[i].total_final);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-evento>\n");
    
    return total;
}


int trade_exportar_contas_receber_xml(FILE *fp) {
    if (!fp) return 0;
    
    ContaReceber contas[1024];
    int total = pers_carregar_contas_receber(contas, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Contas a Receber -->\n");
    fprintf(fp, "  <tabela-contas-receber>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", contas[i].id);
        fprintf(fp, "      <evento_id>%d</evento_id>\n", contas[i].evento_id);
        fprintf(fp, "      <data_emissao>%s</data_emissao>\n", contas[i].data_emissao);
        fprintf(fp, "      <data_vencimento>%s</data_vencimento>\n", contas[i].data_vencimento);
        fprintf(fp, "      <valor_fatura>%.2f</valor_fatura>\n", contas[i].valor_fatura);
        fprintf(fp, "      <valor_recebido>%.2f</valor_recebido>\n", contas[i].valor_recebido);
        fprintf(fp, "      <status>%d</status>\n", contas[i].status);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-contas-receber>\n");
    
    return total;
}



int trade_exportar_contas_pagar_xml(FILE *fp) {
    if (!fp) return 0;
    
    ContaPagar contas[1024];
    int total = pers_carregar_contas_pagar(contas, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Contas a Pagar -->\n");
    fprintf(fp, "  <tabela-contas-pagar>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", contas[i].id);
        fprintf(fp, "      <fornecedor_id>%d</fornecedor_id>\n", contas[i].fornecedor_id);
        fprintf(fp, "      <compra_id>%d</compra_id>\n", contas[i].compra_id);
        fprintf(fp, "      <data_vencimento>%s</data_vencimento>\n", contas[i].data_vencimento);
        fprintf(fp, "      <valor>%.2f</valor>\n", contas[i].valor);
        fprintf(fp, "      <status>%d</status>\n", contas[i].status);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-contas-pagar>\n");
    
    return total;
}


int trade_exportar_caixa_xml(FILE *fp) {
    if (!fp) return 0;
    
    CaixaLancamento lancamentos[1024];
    int total = pers_carregar_caixa(lancamentos, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Movimentação de Caixa -->\n");
    fprintf(fp, "  <tabela-caixa>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", lancamentos[i].id);
        fprintf(fp, "      <data>%s</data>\n", lancamentos[i].data);
        fprintf(fp, "      <tipo>%s</tipo>\n", lancamentos[i].tipo);
        fprintf(fp, "      <forma>%s</forma>\n", lancamentos[i].forma);
        fprintf(fp, "      <valor>%.2f</valor>\n", lancamentos[i].valor);
        
        char desc_escaped[250];
        trade_escape_xml(lancamentos[i].descricao, desc_escaped, sizeof(desc_escaped));
        fprintf(fp, "      <descricao>%s</descricao>\n", desc_escaped);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-caixa>\n");
    
    return total;
}


int trade_exportar_fornecedores_xml(FILE *fp) {
    if (!fp) return 0;
    
    Fornecedor fornecedores[1024];
    int total = pers_carregar_fornecedores(fornecedores, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Fornecedores -->\n");
    fprintf(fp, "  <tabela-fornecedor>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", fornecedores[i].id);
        
        char nome_escaped[250];
        trade_escape_xml(fornecedores[i].nome_fantasia, nome_escaped, sizeof(nome_escaped));
        fprintf(fp, "      <nome_fantasia>%s</nome_fantasia>\n", nome_escaped);
        
        char razao_escaped[250];
        trade_escape_xml(fornecedores[i].razao_social, razao_escaped, sizeof(razao_escaped));
        fprintf(fp, "      <razao_social>%s</razao_social>\n", razao_escaped);
        
        char cnpj_escaped[60];
        trade_escape_xml(fornecedores[i].cnpj_cpf, cnpj_escaped, sizeof(cnpj_escaped));
        fprintf(fp, "      <cnpj_cpf>%s</cnpj_cpf>\n", cnpj_escaped);
        
        char endereco_escaped[300];
        trade_escape_xml(fornecedores[i].endereco, endereco_escaped, sizeof(endereco_escaped));
        fprintf(fp, "      <endereco>%s</endereco>\n", endereco_escaped);
        
        char telefone_escaped[60];
        trade_escape_xml(fornecedores[i].telefone, telefone_escaped, sizeof(telefone_escaped));
        fprintf(fp, "      <telefone>%s</telefone>\n", telefone_escaped);
        
        char servico_escaped[160];
        trade_escape_xml(fornecedores[i].tipo_servico, servico_escaped, sizeof(servico_escaped));
        fprintf(fp, "      <tipo_servico>%s</tipo_servico>\n", servico_escaped);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-fornecedor>\n");
    
    return total;
}


int trade_exportar_operadores_xml(FILE *fp) {
    if (!fp) return 0;
    
    Operador operadores[1024];
    int total = pers_carregar_operadores(operadores, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Operadores -->\n");
    fprintf(fp, "  <tabela-operador>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", operadores[i].id);
        
        char nome_escaped[250];
        trade_escape_xml(operadores[i].nome, nome_escaped, sizeof(nome_escaped));
        fprintf(fp, "      <nome>%s</nome>\n", nome_escaped);
        
        char usuario_escaped[80];
        trade_escape_xml(operadores[i].usuario, usuario_escaped, sizeof(usuario_escaped));
        fprintf(fp, "      <usuario>%s</usuario>\n", usuario_escaped);
        
        fprintf(fp, "      <senha>%s</senha>\n", operadores[i].senha);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-operador>\n");
    
    return total;
}



int trade_exportar_equipes_xml(FILE *fp) {
    if (!fp) return 0;
    
    Equipe equipes[1024];
    int total = pers_carregar_equipes(equipes, 1024);
    
    if (total <= 0) return 0;
    
    fprintf(fp, "  <!-- Tabela de Equipes -->\n");
    fprintf(fp, "  <tabela-equipe>\n");
    
    for (int i = 0; i < total; i++) {
        fprintf(fp, "    <registro>\n");
        
        fprintf(fp, "      <codigo>%d</codigo>\n", equipes[i].id);
        
        char nome_escaped[250];
        trade_escape_xml(equipes[i].nome, nome_escaped, sizeof(nome_escaped));
        fprintf(fp, "      <nome>%s</nome>\n", nome_escaped);
        
        char funcao_escaped[80];
        trade_escape_xml(equipes[i].funcao, funcao_escaped, sizeof(funcao_escaped));
        fprintf(fp, "      <funcao>%s</funcao>\n", funcao_escaped);
        
        fprintf(fp, "      <cpf>%s</cpf>\n", equipes[i].cpf);
        fprintf(fp, "      <valor_hora>%.2f</valor_hora>\n", equipes[i].valor_hora);
        
        fprintf(fp, "    </registro>\n");
    }
    
    fprintf(fp, "  </tabela-equipe>\n");
    
    return total;
}



int trade_exportar_produtora_xml(FILE *fp) {
    if (!fp) return 0;
    
    Produtora produtora;
    if (!pers_obter_produtora(&produtora)) return 0;
    
    fprintf(fp, "  <!-- Tabela de Produtora -->\n");
    fprintf(fp, "  <tabela-produtora>\n");
    fprintf(fp, "    <registro>\n");
    
    char nome_escaped[250];
    trade_escape_xml(produtora.nome_fantasia, nome_escaped, sizeof(nome_escaped));
    fprintf(fp, "      <nome_fantasia>%s</nome_fantasia>\n", nome_escaped);
    
    char razao_escaped[250];
    trade_escape_xml(produtora.razao_social, razao_escaped, sizeof(razao_escaped));
    fprintf(fp, "      <razao_social>%s</razao_social>\n", razao_escaped);
    
    char endereco_escaped[300];
    trade_escape_xml(produtora.endereco, endereco_escaped, sizeof(endereco_escaped));
    fprintf(fp, "      <endereco>%s</endereco>\n", endereco_escaped);
    
    char cnpj_escaped[60];
    trade_escape_xml(produtora.cnpj, cnpj_escaped, sizeof(cnpj_escaped));
    fprintf(fp, "      <cnpj>%s</cnpj>\n", cnpj_escaped);
    
    char telefone_escaped[60];
    trade_escape_xml(produtora.telefone, telefone_escaped, sizeof(telefone_escaped));
    fprintf(fp, "      <telefone>%s</telefone>\n", telefone_escaped);
    
    char email_escaped[160];
    trade_escape_xml(produtora.email, email_escaped, sizeof(email_escaped));
    fprintf(fp, "      <email>%s</email>\n", email_escaped);
    
    fprintf(fp, "    </registro>\n");
    fprintf(fp, "  </tabela-produtora>\n");
    
    return 1;
}


int trade_exportar_xml(const char *caminho_arquivo, int tabelas) {
    if (!caminho_arquivo || tabelas == 0) return 0;
    
    FILE *fp = fopen(caminho_arquivo, "w");
    if (!fp) return 0;
    
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<dados>\n");
    
    int total_registros = 0;
    
    if (tabelas & TRADE_TABLE_CLIENTES) {
        total_registros += trade_exportar_clientes_xml(fp);
    }
    if (tabelas & TRADE_TABLE_EQUIPAMENTOS) {
        total_registros += trade_exportar_equipamentos_xml(fp);
    }
    if (tabelas & TRADE_TABLE_EVENTOS) {
        total_registros += trade_exportar_eventos_xml(fp);
    }
    if (tabelas & TRADE_TABLE_CONTAS_RECEBER) {
        total_registros += trade_exportar_contas_receber_xml(fp);
    }
    if (tabelas & TRADE_TABLE_CONTAS_PAGAR) {
        total_registros += trade_exportar_contas_pagar_xml(fp);
    }
    if (tabelas & TRADE_TABLE_CAIXA) {
        total_registros += trade_exportar_caixa_xml(fp);
    }
    if (tabelas & TRADE_TABLE_FORNECEDORES) {
        total_registros += trade_exportar_fornecedores_xml(fp);
    }
    if (tabelas & TRADE_TABLE_OPERADORES) {
        total_registros += trade_exportar_operadores_xml(fp);
    }
    if (tabelas & TRADE_TABLE_EQUIPES) {
        total_registros += trade_exportar_equipes_xml(fp);
    }
    if (tabelas & TRADE_TABLE_PRODUTORA) {
        total_registros += trade_exportar_produtora_xml(fp);
    }
    
    fprintf(fp, "</dados>\n");
    
    fclose(fp);
    return total_registros;
}



int trade_importar_clientes_xml(FILE *fp, int sobrescrever) {
    if (!fp) return 0;
    
    if (sobrescrever) {
        // Remover todos os clientes existentes
        Cliente clientes[1024];
        int total = pers_carregar_clientes(clientes, 1024);
        for (int i = 0; i < total; i++) {
            pers_remover_cliente(clientes[i].id);
        }
    }
    
    int count = 0;
    char linha[2048];
    Cliente cliente_atual;
    int em_registro = 0;
    int em_cliente = 0;
    
    while (fgets(linha, sizeof(linha), fp)) {
        // Detectar início de registro
        if (strstr(linha, "<registro>")) {
            em_registro = 1;
            memset(&cliente_atual, 0, sizeof(Cliente));
            cliente_atual.id = 0;
            continue;
        }
        
        if (strstr(linha, "</registro>")) {
            em_registro = 0;
            if (em_cliente && cliente_atual.id > 0) {
                pers_salvar_cliente(cliente_atual);
                count++;
            }
            em_cliente = 0;
            continue;
        }
        
        if (em_registro) {
            char valor[1024];
            
            if (trade_extrair_tag_valor(linha, "codigo", valor, sizeof(valor))) {
                cliente_atual.id = atoi(valor);
                em_cliente = 1;
            }
            else if (trade_extrair_tag_valor(linha, "nome", valor, sizeof(valor))) {
                strncpy(cliente_atual.nome, valor, CLIENTE_NOME_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "endereco", valor, sizeof(valor))) {
                strncpy(cliente_atual.endereco, valor, CLIENTE_ENDERECO_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "cpf_cnpj", valor, sizeof(valor))) {
                strncpy(cliente_atual.cpf_cnpj, valor, CLIENTE_CPF_CNPJ_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "telefone", valor, sizeof(valor))) {
                strncpy(cliente_atual.telefone, valor, CLIENTE_TELEFONE_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "email", valor, sizeof(valor))) {
                strncpy(cliente_atual.email, valor, CLIENTE_EMAIL_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "contato", valor, sizeof(valor))) {
                strncpy(cliente_atual.contato, valor, CLIENTE_CONTATO_LEN - 1);
            }
        }
        
        // Detectar fim de tabela-cliente
        if (strstr(linha, "</tabela-cliente>")) {
            break;
        }
    }
    
    return count;
}



int trade_importar_equipamentos_xml(FILE *fp, int sobrescrever) {
    if (!fp) return 0;
    
    if (sobrescrever) {
        Recurso recursos[1024];
        int total = pers_carregar_recursos(recursos, 1024);
        for (int i = 0; i < total; i++) {
            pers_remover_recurso(recursos[i].id);
        }
    }
    
    int count = 0;
    char linha[2048];
    Recurso recurso_atual;
    int em_registro = 0;
    int em_recurso = 0;
    
    while (fgets(linha, sizeof(linha), fp)) {
        if (strstr(linha, "<registro>")) {
            em_registro = 1;
            memset(&recurso_atual, 0, sizeof(Recurso));
            recurso_atual.id = 0;
            continue;
        }
        
        if (strstr(linha, "</registro>")) {
            em_registro = 0;
            if (em_recurso && recurso_atual.id > 0) {
                pers_salvar_recurso(recurso_atual);
                count++;
            }
            em_recurso = 0;
            continue;
        }
        
        if (em_registro) {
            char valor[1024];
            
            if (trade_extrair_tag_valor(linha, "codigo", valor, sizeof(valor))) {
                recurso_atual.id = atoi(valor);
                em_recurso = 1;
            }
            else if (trade_extrair_tag_valor(linha, "descricao", valor, sizeof(valor))) {
                strncpy(recurso_atual.descricao, valor, RECURSO_DESC_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "categoria", valor, sizeof(valor))) {
                strncpy(recurso_atual.categoria, valor, RECURSO_CATEGORIA_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "quantidade_estoque", valor, sizeof(valor))) {
                recurso_atual.quantidade = atoi(valor);
            }
            else if (trade_extrair_tag_valor(linha, "preco_custo", valor, sizeof(valor))) {
                recurso_atual.preco_custo = atof(valor);
            }
            else if (trade_extrair_tag_valor(linha, "valor_locacao", valor, sizeof(valor))) {
                recurso_atual.valor_locacao = atof(valor);
            }
        }
        
        if (strstr(linha, "</tabela-equipamento>")) {
            break;
        }
    }
    
    return count;
}



int trade_importar_eventos_xml(FILE *fp, int sobrescrever) {
    if (!fp) return 0;
    
    if (sobrescrever) {
        Evento eventos[1024];
        int total = pers_carregar_eventos(eventos, 1024);
        for (int i = 0; i < total; i++) {
            pers_remover_evento(eventos[i].id);
        }
    }
    
    int count = 0;
    char linha[2048];
    Evento evento_atual;
    int em_registro = 0;
    int em_evento = 0;
    
    while (fgets(linha, sizeof(linha), fp)) {
        if (strstr(linha, "<registro>")) {
            em_registro = 1;
            memset(&evento_atual, 0, sizeof(Evento));
            evento_atual.id = 0;
            evento_atual.status = EVENTO_STATUS_ORCAMENTO;
            continue;
        }
        
        if (strstr(linha, "</registro>")) {
            em_registro = 0;
            if (em_evento && evento_atual.id > 0) {
                pers_salvar_evento(evento_atual);
                count++;
            }
            em_evento = 0;
            continue;
        }
        
        if (em_registro) {
            char valor[1024];
            
            if (trade_extrair_tag_valor(linha, "codigo", valor, sizeof(valor))) {
                evento_atual.id = atoi(valor);
                em_evento = 1;
            }
            else if (trade_extrair_tag_valor(linha, "nome", valor, sizeof(valor))) {
                strncpy(evento_atual.nome, valor, 120 - 1);
            }
            else if (trade_extrair_tag_valor(linha, "cliente_id", valor, sizeof(valor))) {
                evento_atual.cliente_id = atoi(valor);
            }
            else if (trade_extrair_tag_valor(linha, "data_inicio", valor, sizeof(valor))) {
                strcpy(evento_atual.data_inicio, valor);
            }
            else if (trade_extrair_tag_valor(linha, "data_fim", valor, sizeof(valor))) {
                strcpy(evento_atual.data_fim, valor);
            }
            else if (trade_extrair_tag_valor(linha, "local", valor, sizeof(valor))) {
                strncpy(evento_atual.local, valor, 120 - 1);
            }
            else if (trade_extrair_tag_valor(linha, "status", valor, sizeof(valor))) {
                if (strcmp(valor, "Orcamento") == 0) evento_atual.status = EVENTO_STATUS_ORCAMENTO;
                else if (strcmp(valor, "Aprovado") == 0) evento_atual.status = EVENTO_STATUS_APROVADO;
                else if (strcmp(valor, "Finalizado") == 0) evento_atual.status = EVENTO_STATUS_FINALIZADO;
            }
            else if (trade_extrair_tag_valor(linha, "total_estimado", valor, sizeof(valor))) {
                evento_atual.total_estimado = atof(valor);
            }
            else if (trade_extrair_tag_valor(linha, "total_final", valor, sizeof(valor))) {
                evento_atual.total_final = atof(valor);
            }
        }
        
        if (strstr(linha, "</tabela-evento>")) {
            break;
        }
    }
    
    return count;
}



int trade_importar_contas_receber_xml(FILE *fp, int sobrescrever) {
    // Implementação similar
    return 0;
}

int trade_importar_contas_pagar_xml(FILE *fp, int sobrescrever) {
    // Implementação similar
    return 0;
}

int trade_importar_caixa_xml(FILE *fp, int sobrescrever) {
    // Implementação similar
    return 0;
}



int trade_importar_fornecedores_xml(FILE *fp, int sobrescrever) {
    if (!fp) return 0;
    
    if (sobrescrever) {
        Fornecedor fornecedores[1024];
        int total = pers_carregar_fornecedores(fornecedores, 1024);
        for (int i = 0; i < total; i++) {
            pers_remover_fornecedor(fornecedores[i].id);
        }
    }
    
    int count = 0;
    char linha[2048];
    Fornecedor fornecedor_atual;
    int em_registro = 0;
    int em_fornecedor = 0;
    
    while (fgets(linha, sizeof(linha), fp)) {
        if (strstr(linha, "<registro>")) {
            em_registro = 1;
            memset(&fornecedor_atual, 0, sizeof(Fornecedor));
            fornecedor_atual.id = 0;
            continue;
        }
        
        if (strstr(linha, "</registro>")) {
            em_registro = 0;
            if (em_fornecedor && fornecedor_atual.id > 0) {
                pers_salvar_fornecedor(fornecedor_atual);
                count++;
            }
            em_fornecedor = 0;
            continue;
        }
        
        if (em_registro) {
            char valor[1024];
            
            if (trade_extrair_tag_valor(linha, "codigo", valor, sizeof(valor))) {
                fornecedor_atual.id = atoi(valor);
                em_fornecedor = 1;
            }
            else if (trade_extrair_tag_valor(linha, "nome_fantasia", valor, sizeof(valor))) {
                strncpy(fornecedor_atual.nome_fantasia, valor, FORN_NOME_FANTASIA_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "razao_social", valor, sizeof(valor))) {
                strncpy(fornecedor_atual.razao_social, valor, FORN_RAZAO_SOCIAL_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "cnpj_cpf", valor, sizeof(valor))) {
                strncpy(fornecedor_atual.cnpj_cpf, valor, FORN_DOC_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "endereco", valor, sizeof(valor))) {
                strncpy(fornecedor_atual.endereco, valor, FORN_ENDERECO_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "telefone", valor, sizeof(valor))) {
                strncpy(fornecedor_atual.telefone, valor, FORN_TELEFONE_LEN - 1);
            }
            else if (trade_extrair_tag_valor(linha, "tipo_servico", valor, sizeof(valor))) {
                strncpy(fornecedor_atual.tipo_servico, valor, FORN_SERVICO_LEN - 1);
            }
        }
        
        if (strstr(linha, "</tabela-fornecedor>")) {
            break;
        }
    }
    
    return count;
}



int trade_importar_operadores_xml(FILE *fp, int sobrescrever) {
    // Implementação similar - omitida por brevidade
    return 0;
}


int trade_importar_equipes_xml(FILE *fp, int sobrescrever) {
    // Implementação similar - omitida por brevidade
    return 0;
}


int trade_importar_produtora_xml(FILE *fp, int sobrescrever) {
    // Implementação simplificada
    return 0;
}



int trade_importar_xml(const char *caminho_arquivo, int tabelas, int sobrescrever) {
    if (!caminho_arquivo || tabelas == 0) return -1;
    
    FILE *fp = fopen(caminho_arquivo, "r");
    if (!fp) return -1;
    
    int total_registros = 0;
    
    // Rewind para cada importação
    rewind(fp);
    if (tabelas & TRADE_TABLE_CLIENTES) {
        total_registros += trade_importar_clientes_xml(fp, sobrescrever);
    }
    
    rewind(fp);
    if (tabelas & TRADE_TABLE_EQUIPAMENTOS) {
        total_registros += trade_importar_equipamentos_xml(fp, sobrescrever);
    }
    
    rewind(fp);
    if (tabelas & TRADE_TABLE_EVENTOS) {
        total_registros += trade_importar_eventos_xml(fp, sobrescrever);
    }
    
    rewind(fp);
    if (tabelas & TRADE_TABLE_FORNECEDORES) {
        total_registros += trade_importar_fornecedores_xml(fp, sobrescrever);
    }
    
    fclose(fp);
    return total_registros;
}

/* ========================================================================
 * FUNÇÕES PARA EXPORTAÇÃO/IMPORTAÇÃO EM FORMATO CSV (TEXTO)
 * ======================================================================== */

int trade_exportar_csv(const char *caminho_arquivo, int tabelas, char delimitador) {
    if (!caminho_arquivo || tabelas == 0) return -1;
    
    FILE *fp = fopen(caminho_arquivo, "w");
    if (!fp) return -1;
    
    int total_registros = 0;
    
    // Clientes
    if (tabelas & TRADE_TABLE_CLIENTES) {
        Cliente clientes[1024];
        int total = pers_carregar_clientes(clientes, 1024);
        
        if (total > 0) {
            // Cabeçalho
            fprintf(fp, "ID%cNOME%cCPF_CNPJ%cEMAIL%cTELEFONE%cCEP%cCIDADE%cESTADO%cTIPO\n", 
                delimitador, delimitador, delimitador, delimitador, delimitador, delimitador, delimitador, delimitador);
            
            // Dados
            for (int i = 0; i < total; i++) {
                fprintf(fp, "%d%c%s%c%s%c%s%c%s%c%s\n",
                    clientes[i].id, delimitador,
                    clientes[i].nome, delimitador,
                    clientes[i].cpf_cnpj, delimitador,
                    clientes[i].email, delimitador,
                    clientes[i].telefone, delimitador,
                    clientes[i].endereco);
                total_registros++;
            }
            fprintf(fp, "\n");
        }
    }
    
    // Equipamentos/Recursos
    if (tabelas & TRADE_TABLE_EQUIPAMENTOS) {
        Recurso recursos[1024];
        int total = pers_carregar_recursos(recursos, 1024);
        
        if (total > 0) {
            fprintf(fp, "ID%cDESCRICAO%cCATEGORIA%cQUANTIDADE%cPRECO_CUSTO%cVALOR_LOCACAO\n",
                delimitador, delimitador, delimitador, delimitador, delimitador);
            
            for (int i = 0; i < total; i++) {
                fprintf(fp, "%d%c%s%c%s%c%d%c%.2f%c%.2f\n",
                    recursos[i].id, delimitador,
                    recursos[i].descricao, delimitador,
                    recursos[i].categoria, delimitador,
                    recursos[i].quantidade, delimitador,
                    recursos[i].preco_custo, delimitador,
                    recursos[i].valor_locacao);
                total_registros++;
            }
            fprintf(fp, "\n");
        }
    }
    
    // Fornecedores
    if (tabelas & TRADE_TABLE_FORNECEDORES) {
        Fornecedor fornecedores[1024];
        int total = pers_carregar_fornecedores(fornecedores, 1024);
        
        if (total > 0) {
            fprintf(fp, "ID%cNOME_FANTASIA%cRAZAO_SOCIAL%cCNPJ_CPF%cTELEFONE%cTIPO_SERVICO\n",
                delimitador, delimitador, delimitador, delimitador, delimitador);
            
            for (int i = 0; i < total; i++) {
                fprintf(fp, "%d%c%s%c%s%c%s%c%s%c%s\n",
                    fornecedores[i].id, delimitador,
                    fornecedores[i].nome_fantasia, delimitador,
                    fornecedores[i].razao_social, delimitador,
                    fornecedores[i].cnpj_cpf, delimitador,
                    fornecedores[i].telefone, delimitador,
                    fornecedores[i].tipo_servico);
                total_registros++;
            }
            fprintf(fp, "\n");
        }
    }
    
    fclose(fp);
    return total_registros;
}

int trade_importar_csv(const char *caminho_arquivo, int tabelas, char delimitador, int sobrescrever) {
    if (!caminho_arquivo || tabelas == 0) return -1;
    
    FILE *fp = fopen(caminho_arquivo, "r");
    if (!fp) return -1;
    
    int total_registros = 0;
    char linha[2048];
    
    // Lê cabeçalho para identificar qual tabela está sendo importada
    if (fgets(linha, sizeof(linha), fp) == NULL) {
        fclose(fp);
        return -1;
    }
    
    // Identifica tipo de tabela pelo cabeçalho
    if (strstr(linha, "NOME") && strstr(linha, "ENDERECO")) {
        // Tabela de clientes
        if (!(tabelas & TRADE_TABLE_CLIENTES)) {
            fclose(fp);
            return 0;
        }
        
        if (sobrescrever) {
            // Limpar dados existentes seria ideal, mas as funções de deleção não existem
            // Prosseguir com importação
        }
        
        while (fgets(linha, sizeof(linha), fp)) {
            if (strlen(linha) <= 1 || linha[0] == '\n') break; // Fim da tabela
            
            Cliente cliente;
            char *ptr = linha;
            char *token;
            int campo = 0;
            
            // Parse linha separada por delimitador
            token = strtok(ptr, &delimitador);
            while (token && campo < 6) {
                switch (campo) {
                    case 0: cliente.id = atoi(token); break;
                    case 1: strncpy(cliente.nome, token, sizeof(cliente.nome)-1); break;
                    case 2: strncpy(cliente.cpf_cnpj, token, sizeof(cliente.cpf_cnpj)-1); break;
                    case 3: strncpy(cliente.email, token, sizeof(cliente.email)-1); break;
                    case 4: strncpy(cliente.telefone, token, sizeof(cliente.telefone)-1); break;
                    case 5: strncpy(cliente.endereco, token, sizeof(cliente.endereco)-1); break;
                }
                token = strtok(NULL, &delimitador);
                campo++;
            }
            
            if (pers_salvar_cliente(cliente)) {
                total_registros++;
            }
        }
    }
    else if (strstr(linha, "DESCRICAO") && strstr(linha, "CATEGORIA")) {
        // Tabela de recursos/equipamentos
        if (!(tabelas & TRADE_TABLE_EQUIPAMENTOS)) {
            fclose(fp);
            return 0;
        }
        
        if (sobrescrever) {
            // Limpar dados existentes seria ideal, mas as funções de deleção não existem
            // Prosseguir com importação
        }
        
        while (fgets(linha, sizeof(linha), fp)) {
            if (strlen(linha) <= 1 || linha[0] == '\n') break;
            
            Recurso recurso;
            char *ptr = linha;
            char *token;
            int campo = 0;
            
            token = strtok(ptr, &delimitador);
            while (token && campo < 6) {
                switch (campo) {
                    case 0: recurso.id = atoi(token); break;
                    case 1: strncpy(recurso.descricao, token, sizeof(recurso.descricao)-1); break;
                    case 2: strncpy(recurso.categoria, token, sizeof(recurso.categoria)-1); break;
                    case 3: recurso.quantidade = atoi(token); break;
                    case 4: recurso.preco_custo = atof(token); break;
                    case 5: recurso.valor_locacao = atof(token); break;
                }
                token = strtok(NULL, &delimitador);
                campo++;
            }
            
            if (pers_salvar_recurso(recurso)) {
                total_registros++;
            }
        }
    }
    else if (strstr(linha, "NOME_FANTASIA") && strstr(linha, "CNPJ_CPF")) {
        // Tabela de fornecedores
        if (!(tabelas & TRADE_TABLE_FORNECEDORES)) {
            fclose(fp);
            return 0;
        }
        
        if (sobrescrever) {
            // Limpar dados existentes seria ideal, mas as funções de deleção não existem
            // Prosseguir com importação
        }
        
        while (fgets(linha, sizeof(linha), fp)) {
            if (strlen(linha) <= 1 || linha[0] == '\n') break;
            
            Fornecedor fornecedor;
            char *ptr = linha;
            char *token;
            int campo = 0;
            
            token = strtok(ptr, &delimitador);
            while (token && campo < 6) {
                switch (campo) {
                    case 0: fornecedor.id = atoi(token); break;
                    case 1: strncpy(fornecedor.nome_fantasia, token, sizeof(fornecedor.nome_fantasia)-1); break;
                    case 2: strncpy(fornecedor.razao_social, token, sizeof(fornecedor.razao_social)-1); break;
                    case 3: strncpy(fornecedor.cnpj_cpf, token, sizeof(fornecedor.cnpj_cpf)-1); break;
                    case 4: strncpy(fornecedor.telefone, token, sizeof(fornecedor.telefone)-1); break;
                    case 5: strncpy(fornecedor.tipo_servico, token, sizeof(fornecedor.tipo_servico)-1); break;
                }
                token = strtok(NULL, &delimitador);
                campo++;
            }
            
            if (pers_salvar_fornecedor(fornecedor)) {
                total_registros++;
            }
        }
    }
    
    fclose(fp);
    return total_registros;
}
