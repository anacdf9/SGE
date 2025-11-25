# Documentação - Camada Model

Estruturas de dados e persistência

---

## Visão Geral

A camada Model é responsável por:
- Definir estruturas de dados (structs)
- Gerenciar persistência (salvar/carregar dados)
- Fornecer interface para acesso aos dados

**Não contém:** lógica de negócio, validações complexas, interface gráfica

---

## Sistema de Persistência

### Arquitetura

O sistema usa ponteiros de função para abstrair o tipo de persistência:

```c
typedef struct {
    void* (*criar)(void* dados);
    void* (*ler)(int id);
    void  (*atualizar)(void* dados);
    void  (*deletar)(int id);
    void* (*listar)();
} PersistenciaInterface;
```

### Implementações Disponíveis

**1. Texto (CSV)**
- Arquivo: `pers_texto.c/h`
- Formato: campos separados por `;`
- Arquivo por entidade: `clientes.txt`, `eventos.txt`, etc.

**2. Memória (RAM)**
- Arquivo: `pers_memoria.c/h`
- Arrays estáticos em memória
- Dados perdidos ao fechar

**3. Binário**
- Arquivo: `pers_binario.c/h`
- Usa `fwrite()` e `fread()`
- Arquivos: `clientes.bin`, `eventos.bin`, etc.

### Configuração

Edite `src/config.h`:
```c
#define TIPO_PERSISTENCIA 1  // 1=texto, 2=memoria, 3=binario
```

---

## Estruturas de Dados

### Cliente (cliente.h)

```c
typedef struct {
    int id;
    char nome[100];
    char cpf_cnpj[20];
    char telefone[20];
    char email[100];
    int tipo_pessoa;  // 0=PF, 1=PJ
    int ativo;
} Cliente;
```

**Funções:**
- `cliente_criar(Cliente* c)`
- `cliente_ler(int id)`
- `cliente_atualizar(Cliente* c)`
- `cliente_deletar(int id)`
- `cliente_listar()`

### Recurso (recurso.h/c)

```c
typedef struct {
    int id;
    char nome[100];
    char descricao[200];
    int quantidade;
    float preco_locacao;
    int ativo;
} Recurso;
```

### Evento (evento.h/c)

```c
typedef struct {
    int id;
    char nome[100];
    int id_cliente;
    char data_inicio[11];  // DD/MM/AAAA
    char data_fim[11];
    char hora_inicio[6];   // HH:MM
    char hora_fim[6];
    char local[200];
    char descricao[500];
    int status;  // 0=Orcamento, 1=Aprovado, 2=Finalizado
    float valor_total;
} Evento;
```

### Equipe (equipe.h)

```c
typedef struct {
    int id;
    char nome[100];
    char funcao[100];
    char telefone[20];
    float valor_hora;
    int ativo;
} Equipe;
```

### Fornecedor (fornecedor.h)

```c
typedef struct {
    int id;
    char nome[100];
    char cnpj[20];
    char tipo_servico[100];
    char telefone[20];
    char email[100];
    int ativo;
} Fornecedor;
```

### Produtora (produtora.h)

```c
typedef struct {
    char nome[100];
    char cnpj[20];
    char telefone[20];
    char email[100];
    char endereco[200];
    float margem_lucro;  // Porcentagem (ex: 10.0 = 10%)
} Produtora;
```

### Operador (operador.h)

```c
typedef struct {
    int id;
    char nome[100];
    char login[50];
    char senha[50];
    int nivel;  // 0=Admin, 1=Usuario
    int ativo;
} Operador;
```

---

## Estruturas de Alocação

### EventoItem (evento_item.h/c)

Recursos alocados em um evento:

```c
typedef struct {
    int id;
    int id_evento;
    int id_recurso;
    int quantidade;
    float valor_unitario;
    float valor_total;
} EventoItem;
```

### EventoEquipe (evento_equipe.h)

Equipe alocada em um evento:

```c
typedef struct {
    int id;
    int id_evento;
    int id_equipe;
    float horas;
    float valor_hora;
    float valor_total;
} EventoEquipe;
```

### EventoFornecedor (evento_fornecedor.h)

Fornecedores contratados:

```c
typedef struct {
    int id;
    int id_evento;
    int id_fornecedor;
    char descricao_servico[200];
    float valor;
} EventoFornecedor;
```

---

## Estruturas Financeiras (transacoes.h)

### Caixa

```c
typedef struct {
    int id;
    char data[11];       // DD/MM/AAAA
    int tipo;            // 0=Entrada, 1=Saida
    float valor;
    char descricao[200];
    char categoria[50];
} Caixa;
```

### Compra

```c
typedef struct {
    int id;
    int id_fornecedor;
    char data[11];
    float valor_total;
    char tipo_pagamento[20];  // "A vista" ou "Prazo"
    int parcelas;
} Compra;
```

### CompraItem

```c
typedef struct {
    int id;
    int id_compra;
    int id_recurso;
    int quantidade;
    float custo_unitario;
    float frete_total;
    float imposto_total;
    float preco_locacao_calculado;
} CompraItem;
```

### ContaReceber

```c
typedef struct {
    int id;
    int id_evento;
    char data_emissao[11];
    char data_vencimento[11];
    float valor_total;
    float valor_recebido;
    int status;  // 0=Aberto, 1=Parcial, 2=Liquidado
    int parcelas;
    int parcela_atual;
} ContaReceber;
```

### ContaPagar

```c
typedef struct {
    int id;
    int id_compra;
    char data_vencimento[11];
    float valor_parcela;
    int pago;  // 0=Nao, 1=Sim
    char data_pagamento[11];
    int numero_parcela;
} ContaPagar;
```

---

## Formato de Arquivo Texto

Exemplo: `clientes.txt`

```
1;João Silva;123.456.789-00;(11)91234-5678;joao@email.com;0;1
2;Empresa XYZ;12.345.678/0001-99;(11)3456-7890;contato@xyz.com.br;1;1
```

Campos separados por `;`, um registro por linha.

---

## Funções Padrão

Cada entidade implementa:

### Criar
```c
Cliente* cliente_criar(Cliente* c) {
    // Atribui ID
    // Valida campos obrigatórios
    // Chama persistência->criar()
    // Retorna ponteiro ou NULL
}
```

### Ler
```c
Cliente* cliente_ler(int id) {
    // Chama persistência->ler(id)
    // Retorna ponteiro ou NULL
}
```

### Atualizar
```c
void cliente_atualizar(Cliente* c) {
    // Chama persistência->atualizar(c)
}
```

### Deletar
```c
void cliente_deletar(int id) {
    // Chama persistência->deletar(id)
}
```

### Listar
```c
Cliente* cliente_listar(int* total) {
    // Chama persistência->listar()
    // Retorna array e preenche total
}
```

---

## Gerenciamento de Memória

### Alocação

```c
Cliente* c = malloc(sizeof(Cliente));
if (c == NULL) {
    // Erro
    return NULL;
}
```

### Liberação

```c
if (c != NULL) {
    free(c);
    c = NULL;
}
```

### Arrays

```c
Cliente* lista = malloc(sizeof(Cliente) * total);
// Usar...
free(lista);
```

---

## Constantes (config.h)

```c
#define MAX_CLIENTES 1000
#define MAX_RECURSOS 500
#define MAX_EVENTOS 500
#define MAX_EQUIPES 100
#define MAX_FORNECEDORES 200
#define MAX_OPERADORES 50

#define ARQUIVO_CLIENTES "clientes.txt"
#define ARQUIVO_RECURSOS "recursos.txt"
#define ARQUIVO_EVENTOS "eventos.txt"
// etc...
```

---

## Validações Básicas

O Model faz validações simples:
- ID não pode ser negativo
- Campos obrigatórios não vazios
- Tipos de dados corretos

Validações complexas ficam no Controller.

---

## IDs Automáticos

Cada entidade mantém um contador de ID:

```c
static int proximo_id = 1;

int gerar_id() {
    return proximo_id++;
}
```

Ao carregar dados, ajusta o contador:
```c
if (c->id >= proximo_id) {
    proximo_id = c->id + 1;
}
```

---

## Persistência Texto - Detalhes

### Salvar (pers_texto.c)

```c
void cliente_salvar_texto(Cliente* c) {
    FILE* f = fopen("clientes.txt", "a");
    fprintf(f, "%d;%s;%s;%s;%s;%d;%d\n",
        c->id, c->nome, c->cpf_cnpj, c->telefone,
        c->email, c->tipo_pessoa, c->ativo);
    fclose(f);
}
```

### Carregar

```c
Cliente* cliente_carregar_texto(int* total) {
    FILE* f = fopen("clientes.txt", "r");
    Cliente* lista = malloc(sizeof(Cliente) * MAX_CLIENTES);
    int count = 0;
    
    char linha[500];
    while (fgets(linha, 500, f)) {
        sscanf(linha, "%d;%[^;];%[^;];%[^;];%[^;];%d;%d",
            &lista[count].id, lista[count].nome, ...);
        count++;
    }
    
    fclose(f);
    *total = count;
    return lista;
}
```

---

## Persistência Binária - Detalhes

### Salvar

```c
void cliente_salvar_binario(Cliente* c) {
    FILE* f = fopen("clientes.bin", "ab");
    fwrite(c, sizeof(Cliente), 1, f);
    fclose(f);
}
```

### Carregar

```c
Cliente* cliente_carregar_binario(int* total) {
    FILE* f = fopen("clientes.bin", "rb");
    
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    int count = tamanho / sizeof(Cliente);
    rewind(f);
    
    Cliente* lista = malloc(sizeof(Cliente) * count);
    fread(lista, sizeof(Cliente), count, f);
    
    fclose(f);
    *total = count;
    return lista;
}
```

---

## Boas Práticas

1. **Sempre validar ponteiros:**
```c
if (c == NULL) return NULL;
```

2. **Liberar memória alocada:**
```c
free(c);
```

3. **Fechar arquivos:**
```c
fclose(f);
```

4. **Verificar abertura de arquivos:**
```c
FILE* f = fopen("arquivo.txt", "r");
if (f == NULL) return NULL;
```

5. **Usar constantes:**
```c
#define MAX_NOME 100
char nome[MAX_NOME];
```

---

## Como Adicionar Nova Entidade

1. Criar `nova_entidade.h`:
```c
typedef struct {
    int id;
    // campos...
} NovaEntidade;

NovaEntidade* nova_entidade_criar(NovaEntidade* ne);
NovaEntidade* nova_entidade_ler(int id);
void nova_entidade_atualizar(NovaEntidade* ne);
void nova_entidade_deletar(int id);
NovaEntidade* nova_entidade_listar(int* total);
```

2. Criar `nova_entidade.c`:
Implementar as funções usando a interface de persistência.

3. Adicionar em `config.h`:
```c
#define MAX_NOVA_ENTIDADE 100
#define ARQUIVO_NOVA_ENTIDADE "nova_entidade.txt"
```

4. Adicionar no Makefile.

---

## Arquivos do Model

```
src/model/
├── pers.c/h              # Interface de persistência
├── pers_texto.c/h        # Implementação texto
├── pers_memoria.c/h      # Implementação memória
├── pers_binario.c/h      # Implementação binário
├── cliente.c/h           # Entidade cliente
├── recurso.c/h           # Entidade recurso
├── evento.c/h            # Entidade evento
├── evento_item.c/h       # Itens de evento
├── alocacao.c/h          # Alocações
├── equipe.h              # Entidade equipe
├── fornecedor.h          # Entidade fornecedor
├── produtora.h           # Entidade produtora
├── operador.h            # Entidade operador
└── transacoes.h          # Estruturas financeiras
```

---

## Dependências

Model depende apenas de:
- stdlib.h (malloc, free)
- stdio.h (FILE, fprintf, fscanf)
- string.h (strcmp, strcpy)
- config.h (constantes)

Não depende de IUP ou outras camadas.

---

## Fluxo de Dados

```
Controller chama Model
    ↓
Model chama Persistência
    ↓
Persistência (texto/memória/binário)
    ↓
Arquivo/RAM
    ↓
Persistência retorna dados
    ↓
Model retorna para Controller
```

---

## Testes

Para testar o Model sem interface:

```c
// main_teste.c
#include "src/model/cliente.h"

int main() {
    Cliente c;
    c.id = 1;
    strcpy(c.nome, "Teste");
    strcpy(c.cpf_cnpj, "123.456.789-00");
    c.ativo = 1;
    
    cliente_criar(&c);
    
    Cliente* lido = cliente_ler(1);
    printf("Nome: %s\n", lido->nome);
    
    return 0;
}
```

---

## Veja Também

- `src/controller/README.md` - Lógica de negócio
- `src/view/README.md` - Interface gráfica
- `DOCUMENTACAO.md` - Índice completo
