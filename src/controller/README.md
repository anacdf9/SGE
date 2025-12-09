# Documentação - Camada Controller

Lógica de negócio e regras

---

## Visão Geral

A camada Controller é responsável por:
- Implementar regras de negócio
- Validar dados complexos
- Realizar cálculos
- Conectar Model e View
- Orquestrar operações

**Não contém:** persistência direta, código de interface

---

## Princípios

1. **Controller não acessa arquivos diretamente**
   - Usa o Model para persistência

2. **Controller não cria widgets**
   - Retorna dados para a View

3. **Controller concentra a lógica**
   - CPF válido? Controller
   - Calcular total? Controller
   - Recurso disponível? Controller

---

## Controllers Disponíveis

### cliente_controller.c/h

**Funções:**
```c
int cliente_validar_cpf_cnpj(char* cpf_cnpj);
int cliente_validar_email(char* email);
int cliente_validar_telefone(char* telefone);
Cliente* cliente_buscar_por_cpf(char* cpf_cnpj);
int cliente_tem_eventos(int id_cliente);
```

**Responsabilidades:**
- Validar CPF/CNPJ (dígitos verificadores)
- Validar formato de email
- Validar formato de telefone
- Buscar cliente por CPF
- Verificar se cliente tem eventos (antes de deletar)

### recurso_controller.c/h

**Funções:**
```c
int recurso_verificar_disponibilidade(int id_recurso, int quantidade);
float recurso_calcular_valor_locacao(int id_recurso, int quantidade, int dias);
void recurso_atualizar_estoque(int id_recurso, int quantidade, int operacao);
Recurso* recurso_buscar_por_nome(char* nome);
```

**Responsabilidades:**
- Verificar se tem estoque disponível
- Calcular valor de locação (quantidade × preço × dias)
- Atualizar estoque (adicionar/remover)
- Buscar recurso por nome

### evento_controller.c/h

**Funções:**
```c
int evento_validar_datas(char* data_inicio, char* data_fim);
float evento_calcular_total(int id_evento);
int evento_mudar_status(int id_evento, int novo_status);
int evento_gerar_conta_receber(int id_evento, int parcelas);
EventoItem* evento_listar_itens(int id_evento, int* total);
EventoEquipe* evento_listar_equipe(int id_evento, int* total);
```

**Responsabilidades:**
- Validar se data fim >= data início
- Calcular total do evento (recursos + equipe + fornecedores)
- Mudar status (orçamento → aprovado → finalizado)
- Gerar conta a receber ao finalizar
- Listar itens, equipe, fornecedores do evento

### transacoes_controller.c/h

**Funções principais:**

**Caixa:**
```c
float caixa_calcular_saldo();
Caixa* caixa_listar_periodo(char* data_ini, char* data_fim, int* total);
void caixa_lancar_entrada(float valor, char* descricao, char* categoria);
void caixa_lancar_saida(float valor, char* descricao, char* categoria);
```

**Compras:**
```c
int compra_registrar(Compra* compra, CompraItem* itens, int num_itens);
float compra_calcular_preco_locacao(CompraItem* item, float margem_lucro);
void compra_gerar_contas_pagar(int id_compra);
```

**Contas a Receber:**
```c
int conta_receber_registrar_pagamento(int id, float valor);
int conta_receber_verificar_status(int id);
ContaReceber* conta_receber_listar_por_status(int status, int* total);
```

**Contas a Pagar:**
```c
int conta_pagar_baixar(int id);
ContaPagar* conta_pagar_listar_vencidas(int* total);
ContaPagar* conta_pagar_listar_por_compra(int id_compra, int* total);
```

### equipe_controller.c/h

**Funções:**
```c
float equipe_calcular_custo(int id_equipe, float horas);
int equipe_verificar_disponibilidade(int id_equipe, char* data);
Equipe* equipe_listar_ativos(int* total);
```

### fornecedor_controller.c/h

**Funções:**
```c
int fornecedor_validar_cnpj(char* cnpj);
Fornecedor* fornecedor_buscar_por_servico(char* tipo_servico, int* total);
int fornecedor_tem_eventos(int id_fornecedor);
```

### produtora_controller.c/h

**Funções:**
```c
int produtora_configurar(Produtora* p);
Produtora* produtora_obter();
float produtora_obter_margem_lucro();
int produtora_validar_dados(Produtora* p);
```

### operador_controller.c/h

**Funções:**
```c
int operador_autenticar(char* login, char* senha);
int operador_alterar_senha(int id, char* senha_atual, char* senha_nova);
char* operador_criptografar_senha(char* senha);
```

### alocacao_controller.c/h

**Funções:**
```c
int alocacao_recurso_adicionar(int id_evento, int id_recurso, int qtd);
int alocacao_recurso_remover(int id_evento, int id_recurso);
int alocacao_equipe_adicionar(int id_evento, int id_equipe, float horas);
int alocacao_fornecedor_adicionar(int id_evento, int id_fornecedor, char* desc, float valor);
```

### feedback_controller.c/h

**Propósito:**
Gera relatórios analíticos de todas as entidades do sistema com filtros avançados e exportação para CSV.

**Estruturas de Filtro:**
```c
typedef struct {
    int filtrar_tipo;        // 0=nenhum, 1=PF, 2=PJ
    int filtrar_ativo;       // -1=todos, 0=inativos, 1=ativos
    char data_inicio[11];    // YYYY-MM-DD
    char data_fim[11];       // YYYY-MM-DD
} FiltroCliente;

typedef struct {
    int filtrar_status;      // 0=todos, 1=Orçamento, 2=Aprovado, 3=Finalizado
    int id_cliente;          // 0=todos
    float valor_min, valor_max;
    char data_inicio[11];
    char data_fim[11];
} FiltroEvento;
```

**Funções Principais:**
```c
// Gerar relatórios (retorna string formatada)
char* feedback_relatorio_clientes(FiltroCliente* filtro);
char* feedback_relatorio_eventos(FiltroEvento* filtro);
char* feedback_relatorio_recursos(FiltroRecurso* filtro);
char* feedback_relatorio_contas_receber(FiltroContaReceber* filtro);
char* feedback_relatorio_contas_pagar(FiltroContaPagar* filtro);
char* feedback_relatorio_caixa(FiltroCaixa* filtro);
char* feedback_relatorio_resumo(FiltroResumo* filtro);

// Exportar para CSV
int feedback_exportar_clientes_csv(FiltroCliente* filtro, char* arquivo);
int feedback_exportar_eventos_csv(FiltroEvento* filtro, char* arquivo);
// ... outras exportações
```

**Responsabilidades:**
- Filtrar dados baseado em critérios
- Formatar saída com alinhamento e padding
- Gerar cabeçalhos de colunas
- Ordenar alfabeticamente quando necessário
- Exportar para CSV com delimitador `;`
- Validar ranges de datas
- Buscar dados auxiliares (nomes, descrições)

### trade_controller.c/h

**Propósito:**
Importar e exportar dados em formato XML com seleção seletiva de tabelas e validação de caracteres especiais.

**Enumeração de Tabelas:**
```c
typedef enum {
    TRADE_TABLE_CLIENTES      = 0x01,
    TRADE_TABLE_RECURSOS      = 0x02,
    TRADE_TABLE_EVENTOS       = 0x04,
    TRADE_TABLE_FORNECEDORES  = 0x08,
    TRADE_TABLE_EQUIPES       = 0x10,
    TRADE_TABLE_OPERADORES    = 0x20,
    TRADE_TABLE_COMPRAS       = 0x40,
    TRADE_TABLE_CONTAS_REC    = 0x80,
    TRADE_TABLE_CONTAS_PAG    = 0x100,
    TRADE_TABLE_CAIXA         = 0x200
} TradeTable;
```

**Funções Principais:**
```c
// Exportação
int trade_exportar_xml(const char* arquivo, int tabelas_selecionadas);
int trade_exportar_clientes_xml(FILE* fp);
int trade_exportar_recursos_xml(FILE* fp);
// ... outras tabelas

// Importação
int trade_importar_xml(const char* arquivo, int tabelas_selecionadas, int sobrescrever);
int trade_importar_clientes_xml(FILE* fp, int sobrescrever);
int trade_importar_recursos_xml(FILE* fp, int sobrescrever);
// ... outras tabelas

// Utilitários de XML
char* trade_escape_xml(const char* texto);
char* trade_unescape_xml(const char* texto);
char* trade_extrair_tag(const char* xml, const char* tag);
char* trade_gerar_tag(const char* tag, const char* valor);
```

**Responsabilidades:**
- Gerar XML bem formado com estrutura definida
- Escapar caracteres especiais: &, <, >, ", '
- Parsear XML com validação de tags
- Suportar seleção seletiva via flags bitwise
- Validar dados antes da importação
- Oferecer opção de sobrescrita
- Tratamento de conflitos de ID
- Manter integridade referencial

---

## Padrão de Implementação

### Estrutura Típica de um Controller

```c
/* =============================================================================
 * INCLUDES
 * ============================================================================= */
#include "entidade_controller.h"
#include "../model/entidade.h"
#include <string.h>
#include <stdlib.h>

/* =============================================================================
 * FUNÇÕES DE VALIDAÇÃO
 * ============================================================================= */

int entidade_validar_campo(char* campo) {
    // Implementar validação
    if (strlen(campo) == 0) return 0;
    // Outras validações...
    return 1;
}

/* =============================================================================
 * FUNÇÕES DE CÁLCULO
 * ============================================================================= */

float entidade_calcular_valor(int id) {
    // Obter dados do Model
    Entidade* e = entidade_ler(id);
    if (e == NULL) return 0.0;
    
    // Fazer cálculo
    float resultado = e->campo1 * e->campo2;
    
    free(e);
    return resultado;
}

/* =============================================================================
 * FUNÇÕES DE BUSCA
 * ============================================================================= */

Entidade* entidade_buscar_por_criterio(char* criterio) {
    int total;
    Entidade* lista = entidade_listar(&total);
    
    for (int i = 0; i < total; i++) {
        if (strcmp(lista[i].campo, criterio) == 0) {
            Entidade* encontrado = malloc(sizeof(Entidade));
            *encontrado = lista[i];
            free(lista);
            return encontrado;
        }
    }
    
    free(lista);
    return NULL;
}

/* =============================================================================
 * FUNÇÕES DE REGRAS DE NEGÓCIO
 * ============================================================================= */

int entidade_executar_acao(int id) {
    // Validar
    if (id <= 0) return 0;
    
    // Obter dados
    Entidade* e = entidade_ler(id);
    if (e == NULL) return 0;
    
    // Aplicar regra
    e->status = NOVO_STATUS;
    
    // Salvar
    entidade_atualizar(e);
    
    free(e);
    return 1;
}
```

---

## Validações Comuns

### CPF

```c
int validar_cpf(char* cpf) {
    // Remove pontos e traços
    char numeros[12];
    int j = 0;
    for (int i = 0; cpf[i] != '\0'; i++) {
        if (cpf[i] >= '0' && cpf[i] <= '9') {
            numeros[j++] = cpf[i];
        }
    }
    numeros[j] = '\0';
    
    // Deve ter 11 dígitos
    if (strlen(numeros) != 11) return 0;
    
    // Calcular dígitos verificadores
    // (implementação completa...)
    
    return 1;
}
```

### CNPJ

```c
int validar_cnpj(char* cnpj) {
    // Similar ao CPF, mas com 14 dígitos
    // (implementação completa...)
    return 1;
}
```

### Email

```c
int validar_email(char* email) {
    // Deve conter @
    if (strchr(email, '@') == NULL) return 0;
    
    // Deve conter . após @
    char* arroba = strchr(email, '@');
    if (strchr(arroba, '.') == NULL) return 0;
    
    return 1;
}
```

### Data

```c
int validar_data(char* data) {
    // Formato DD/MM/AAAA
    if (strlen(data) != 10) return 0;
    if (data[2] != '/' || data[5] != '/') return 0;
    
    int dia, mes, ano;
    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);
    
    if (dia < 1 || dia > 31) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (ano < 1900 || ano > 2100) return 0;
    
    return 1;
}
```

---

## Cálculos Financeiros

### Preço de Locação

```c
float calcular_preco_locacao(CompraItem* item, float margem) {
    float custo = item->custo_unitario;
    float frete_unit = item->frete_total / item->quantidade;
    float imposto_unit = item->imposto_total / item->quantidade;
    float margem_valor = custo * (margem / 100.0);
    
    return custo + frete_unit + imposto_unit + margem_valor;
}
```

### Total do Evento

```c
float evento_calcular_total(int id_evento) {
    float total = 0.0;
    int count;
    
    // Recursos
    EventoItem* itens = evento_listar_itens(id_evento, &count);
    for (int i = 0; i < count; i++) {
        total += itens[i].valor_total;
    }
    free(itens);
    
    // Equipe
    EventoEquipe* equipes = evento_listar_equipe(id_evento, &count);
    for (int i = 0; i < count; i++) {
        total += equipes[i].valor_total;
    }
    free(equipes);
    
    // Fornecedores
    EventoFornecedor* fornecedores = evento_listar_fornecedores(id_evento, &count);
    for (int i = 0; i < count; i++) {
        total += fornecedores[i].valor;
    }
    free(fornecedores);
    
    return total;
}
```

### Saldo do Caixa

```c
float caixa_calcular_saldo() {
    int total;
    Caixa* lancamentos = caixa_listar(&total);
    
    float saldo = 0.0;
    for (int i = 0; i < total; i++) {
        if (lancamentos[i].tipo == 0) {  // Entrada
            saldo += lancamentos[i].valor;
        } else {  // Saída
            saldo -= lancamentos[i].valor;
        }
    }
    
    free(lancamentos);
    return saldo;
}
```

---

## Regras de Negócio

### Mudar Status de Evento

```c
int evento_mudar_status(int id_evento, int novo_status) {
    Evento* e = evento_ler(id_evento);
    if (e == NULL) return 0;
    
    // Validar transição de status
    if (e->status == STATUS_FINALIZADO) {
        // Não pode alterar evento finalizado
        free(e);
        return 0;
    }
    
    if (novo_status == STATUS_APROVADO) {
        // Verificar se tem recursos alocados
        int count;
        EventoItem* itens = evento_listar_itens(id_evento, &count);
        if (count == 0) {
            // Evento sem recursos
            free(itens);
            free(e);
            return 0;
        }
        free(itens);
    }
    
    // Atualizar status
    e->status = novo_status;
    evento_atualizar(e);
    
    free(e);
    return 1;
}
```

### Verificar Disponibilidade de Recurso

```c
int recurso_verificar_disponibilidade(int id_recurso, int quantidade) {
    Recurso* r = recurso_ler(id_recurso);
    if (r == NULL) return 0;
    
    int disponivel = (r->quantidade >= quantidade);
    
    free(r);
    return disponivel;
}
```

### Gerar Contas a Pagar de uma Compra

```c
void compra_gerar_contas_pagar(int id_compra) {
    Compra* c = compra_ler(id_compra);
    if (c == NULL) return;
    
    if (strcmp(c->tipo_pagamento, "A vista") == 0) {
        // Uma única conta
        ContaPagar cp;
        cp.id_compra = id_compra;
        cp.valor_parcela = c->valor_total;
        cp.numero_parcela = 1;
        cp.pago = 0;
        strcpy(cp.data_vencimento, c->data);
        conta_pagar_criar(&cp);
    } else {
        // Múltiplas parcelas
        float valor_parcela = c->valor_total / c->parcelas;
        for (int i = 1; i <= c->parcelas; i++) {
            ContaPagar cp;
            cp.id_compra = id_compra;
            cp.valor_parcela = valor_parcela;
            cp.numero_parcela = i;
            cp.pago = 0;
            // Calcular data vencimento (data + i meses)
            // ...
            conta_pagar_criar(&cp);
        }
    }
    
    free(c);
}
```

---

## Tratamento de Erros

### Retorno de Funções

**Sucesso/Falha (int):**
```c
int funcao() {
    // Retorna 1 se sucesso, 0 se falha
    if (erro) return 0;
    return 1;
}
```

**Ponteiro:**
```c
Entidade* funcao() {
    // Retorna ponteiro ou NULL
    if (erro) return NULL;
    return ponteiro;
}
```

**Valor:**
```c
float funcao() {
    // Retorna valor ou 0.0/-1.0 para erro
    if (erro) return -1.0;
    return valor;
}
```

### Mensagens de Erro

Controller não exibe mensagens. Retorna código de erro e a View decide o que fazer:

```c
// Controller
int cliente_salvar(Cliente* c) {
    if (!validar_cpf(c->cpf_cnpj)) return ERRO_CPF_INVALIDO;
    if (!validar_email(c->email)) return ERRO_EMAIL_INVALIDO;
    // Salvar...
    return SUCESSO;
}

// View
int resultado = cliente_salvar(&c);
if (resultado == ERRO_CPF_INVALIDO) {
    IupMessage("Erro", "CPF inválido");
} else if (resultado == ERRO_EMAIL_INVALIDO) {
    IupMessage("Erro", "Email inválido");
}
```

---

## Constantes de Erro (config.h)

```c
#define SUCESSO 0
#define ERRO_CPF_INVALIDO -1
#define ERRO_CNPJ_INVALIDO -2
#define ERRO_EMAIL_INVALIDO -3
#define ERRO_DATA_INVALIDA -4
#define ERRO_ESTOQUE_INSUFICIENTE -5
#define ERRO_STATUS_INVALIDO -6
// etc...
```

---

## Boas Práticas

1. **Validar entrada:**
```c
if (ponteiro == NULL) return 0;
if (id <= 0) return 0;
if (strlen(campo) == 0) return 0;
```

2. **Liberar memória:**
```c
Entidade* e = entidade_ler(id);
// usar e...
free(e);
```

3. **Não duplicar código:**
```c
// Extrair função auxiliar
float calcular_algo(int a, int b) {
    return a * b * 1.1;
}
```

4. **Usar constantes:**
```c
#define STATUS_ORCAMENTO 0
#define STATUS_APROVADO 1
```

5. **Separar responsabilidades:**
- Uma função = uma responsabilidade
- Funções pequenas e focadas

---

## Testes

Testar Controller sem interface:

```c
// teste_controller.c
#include "cliente_controller.h"

int main() {
    // Testar validação de CPF
    int valido = cliente_validar_cpf("123.456.789-00");
    printf("CPF válido: %d\n", valido);
    
    // Testar busca
    Cliente* c = cliente_buscar_por_cpf("123.456.789-00");
    if (c != NULL) {
        printf("Cliente encontrado: %s\n", c->nome);
        free(c);
    }
    
    return 0;
}
```

---

## Arquivos do Controller

```
src/controller/
├── cliente_controller.c/h
├── recurso_controller.c/h
├── evento_controller.c/h
├── equipe_controller.c/h
├── fornecedor_controller.c/h
├── produtora_controller.c/h
├── operador_controller.c/h
├── alocacao_controller.c/h
└── transacoes_controller.c/h
```

---

## Dependências

Controller depende de:
- Model (para persistência)
- stdlib.h, string.h, stdio.h
- config.h

Não depende de View ou IUP.

---

## Fluxo Típico

```
View captura entrada do usuário
    ↓
View chama Controller
    ↓
Controller valida dados
    ↓
Controller chama Model
    ↓
Model persiste/lê dados
    ↓
Model retorna para Controller
    ↓
Controller processa/calcula
    ↓
Controller retorna para View
    ↓
View atualiza interface
```

---

## Veja Também

- `src/model/README.md` - Estruturas de dados
- `src/view/README.md` - Interface gráfica
- `DOCUMENTACAO.md` - Índice completo
