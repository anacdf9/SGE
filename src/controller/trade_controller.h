#ifndef TRADE_CONTROLLER_H
#define TRADE_CONTROLLER_H

#include <stdio.h>


typedef enum {
    TRADE_TABLE_CLIENTES = 1 << 0,          // 0x01
    TRADE_TABLE_EQUIPAMENTOS = 1 << 1,      // 0x02
    TRADE_TABLE_EVENTOS = 1 << 2,           // 0x04
    TRADE_TABLE_CONTAS_RECEBER = 1 << 3,    // 0x08
    TRADE_TABLE_CONTAS_PAGAR = 1 << 4,      // 0x10
    TRADE_TABLE_CAIXA = 1 << 5,             // 0x20
    TRADE_TABLE_FORNECEDORES = 1 << 6,      // 0x40
    TRADE_TABLE_OPERADORES = 1 << 7,        // 0x80
    TRADE_TABLE_EQUIPES = 1 << 8,           // 0x100
    TRADE_TABLE_PRODUTORA = 1 << 9,         // 0x200
    
    TRADE_TABLE_ALL = 0x3FF                 // Todas as tabelas
} TradeTable;


/**
 * Exporta dados selecionados para arquivo XML
 * @param caminho_arquivo: caminho completo do arquivo XML a criar
 * @param tabelas: flags indicando quais tabelas exportar (usar OR para múltiplas)
 * @return: 1 se sucesso, 0 se erro
 * 
 * Exemplo de uso:
 *   trade_exportar_xml("dados.xml", TRADE_TABLE_CLIENTES | TRADE_TABLE_EQUIPAMENTOS);
 */
int trade_exportar_xml(const char *caminho_arquivo, int tabelas);


int trade_exportar_clientes_xml(FILE *fp);


int trade_exportar_equipamentos_xml(FILE *fp);


int trade_exportar_eventos_xml(FILE *fp);


int trade_exportar_contas_receber_xml(FILE *fp);


int trade_exportar_contas_pagar_xml(FILE *fp);


int trade_exportar_caixa_xml(FILE *fp);


int trade_exportar_fornecedores_xml(FILE *fp);


int trade_exportar_operadores_xml(FILE *fp);


int trade_exportar_equipes_xml(FILE *fp);


int trade_exportar_produtora_xml(FILE *fp);


/**
 * Importa dados de arquivo XML
 * @param caminho_arquivo: caminho completo do arquivo XML a ler
 * @param tabelas: flags indicando quais tabelas importar (usar OR para múltiplas)
 * @param sobrescrever: 1 para sobrescrever dados existentes, 0 para mesclar
 * @return: número de registros importados, -1 se erro crítico
 */
int trade_importar_xml(const char *caminho_arquivo, int tabelas, int sobrescrever);

/**
 * Importa clientes de XML (já aberto)
 * @param fp: arquivo XML aberto
 * @param sobrescrever: 1 para limpar antes, 0 para mesclar
 * @return: número de clientes importados
 */
int trade_importar_clientes_xml(FILE *fp, int sobrescrever);


int trade_importar_equipamentos_xml(FILE *fp, int sobrescrever);


int trade_importar_eventos_xml(FILE *fp, int sobrescrever);


int trade_importar_contas_receber_xml(FILE *fp, int sobrescrever);


int trade_importar_contas_pagar_xml(FILE *fp, int sobrescrever);


int trade_importar_caixa_xml(FILE *fp, int sobrescrever);

int trade_importar_fornecedores_xml(FILE *fp, int sobrescrever);


int trade_importar_operadores_xml(FILE *fp, int sobrescrever);


int trade_importar_equipes_xml(FILE *fp, int sobrescrever);


int trade_importar_produtora_xml(FILE *fp, int sobrescrever);


/**
 * Escapa caracteres especiais XML (", <, >, &)
 * @param input: string original
 * @param output: buffer para string escapada
 * @param max_len: tamanho máximo do buffer de saída
 */
void trade_escape_xml(const char *input, char *output, int max_len);

/**
 * Desescapa caracteres XML (&quot;, &lt;, &gt;, &amp;)
 * @param input: string com entidades XML
 * @param output: buffer para string desescapada
 * @param max_len: tamanho máximo do buffer de saída
 */
void trade_unescape_xml(const char *input, char *output, int max_len);

/**
 * Extrai valor de uma tag XML
 * Exemplo: <nome>João Silva</nome> -> retorna "João Silva"
 * @param tag: tag XML completa (ex: "<nome>valor</nome>")
 * @param output: buffer para o valor extraído
 * @param max_len: tamanho máximo do buffer
 * @return: 1 se sucesso, 0 se falha
 */
int trade_extrair_tag_valor(const char *tag_line, const char *tag_name, char *output, int max_len);

/**
 * Gera uma tag XML formatada
 * Exemplo: trade_gerar_tag("nome", "João Silva") -> "<nome>João Silva</nome>"
 * @param tag_name: nome da tag
 * @param valor: valor da tag (será escapado automaticamente)
 * @param output: buffer para a tag XML
 * @param max_len: tamanho máximo do buffer
 */
void trade_gerar_tag(const char *tag_name, const char *valor, char *output, int max_len);

#endif // TRADE_CONTROLLER_H
