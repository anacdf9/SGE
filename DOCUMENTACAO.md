# Índice de Documentação

Guia completo do projeto SGE

---

## Documentação Principal

### README.md
Visão geral do projeto
- O que é o SGE
- Arquitetura MVC
- Tecnologias usadas
- Sistema de persistência
- Módulos e funcionalidades
- Como usar (resumo)

### COMPILACAO.md
Guia de compilação
- Requisitos (MinGW, IUP)
- Como compilar
- Makefile explicado
- Problemas comuns e soluções
- Dicas de debug

### EXEMPLOS.md
Exemplos práticos de uso
- Primeiro uso
- Cadastros passo a passo
- Criar eventos
- Transações financeiras
- Fluxo completo de trabalho
- Atalhos úteis

---

## Documentação Técnica

### src/model/README.md
Camada de Dados (Model)
- Estruturas de dados (structs)
- Sistema de persistência
- Arquivos texto, memória e binário
- Como adicionar novos modelos
- Estruturas financeiras

### src/controller/README.md
Camada de Lógica (Controller)
- Regras de negócio
- Validações
- Cálculos
- Controllers disponíveis
- Como adicionar novos controllers

### src/view/README.md
Camada de Interface (View)
- Telas e formulários
- Matrizes (IupMatrix)
- Callbacks
- Validação de entrada
- Como criar novas telas

---

## Arquivos de Configuração

### src/config.h
Configurações globais do sistema
```c
#define TIPO_PERSISTENCIA 1  // 1=texto, 2=memoria, 3=binario
#define MAX_CLIENTES 1000
#define MAX_EVENTOS 500
// etc...
```

### makefile
Configuração de build
- Compilador (GCC)
- Flags de compilação
- Bibliotecas (IUP)
- Regras de build

---

## Arquivos de Dados

Gerados automaticamente pelo sistema (na raiz):

```
clientes.txt           # Cadastro de clientes
recursos.txt           # Equipamentos e estoque
eventos.txt            # Eventos e orçamentos
eventos_itens.txt      # Recursos alocados em eventos
eventos_equipes.txt    # Equipe alocada em eventos
eventos_fornecedores.txt # Fornecedores contratados
caixa.txt             # Lançamentos de caixa
compras.txt           # Compras de equipamentos
compras_itens.txt     # Itens das compras
contas_receber.txt    # Contas a receber
contas_pagar.txt      # Contas a pagar
produtora.txt         # Dados da produtora
equipes.txt           # Equipe interna
fornecedores.txt      # Fornecedores/parceiros
operadores.txt        # Usuários do sistema
```

---

## Estrutura do Código

### Organização dos Arquivos

```
SGE/
├── main.c                    # Programa principal
├── makefile                  # Build
├── *.md                      # Documentação
├── *.txt                     # Dados
└── src/
    ├── config.h              # Configurações
    ├── model/                # Camada de dados
    │   ├── pers.c/h          # Interface de persistência
    │   ├── pers_texto.c/h    # Implementação texto
    │   ├── pers_memoria.c/h  # Implementação memória
    │   ├── pers_binario.c/h  # Implementação binário
    │   ├── cliente.c/h       # Modelo de cliente
    │   ├── evento.c/h        # Modelo de evento
    │   ├── recurso.c/h       # Modelo de recurso
    │   ├── alocacao.c/h      # Alocações
    │   ├── transacoes.h      # Estruturas financeiras
    │   └── README.md
    ├── controller/           # Camada de lógica
    │   ├── cliente_controller.c/h
    │   ├── evento_controller.c/h
    │   ├── recurso_controller.c/h
    │   ├── equipe_controller.c/h
    │   ├── fornecedor_controller.c/h
    │   ├── operador_controller.c/h
    │   ├── produtora_controller.c/h
    │   ├── alocacao_controller.c/h
    │   ├── transacoes_controller.c/h
    │   └── README.md
    └── view/                 # Camada de interface
        ├── cliente_view.c/h
        ├── evento_view.c/h
        ├── recurso_view.c/h
        ├── equipe_view.c/h
        ├── fornecedor_view.c/h
        ├── operador_view.c/h
        ├── produtora_view.c/h
        ├── alocacao_view.c/h
        ├── transacoes_view.c/h
        ├── ui_common.c/h     # Funções comuns de UI
        ├── validation.c/h    # Validações de entrada
        └── README.md
```

---

## Padrões de Código

### Nomenclatura

**Arquivos:**
- Model: `entidade.c/h` (ex: `cliente.c`)
- Controller: `entidade_controller.c/h`
- View: `entidade_view.c/h`

**Funções:**
- Model: `entidade_acao()` (ex: `cliente_criar()`)
- Controller: `entidade_acao_controller()` (ex: `cliente_validar_controller()`)
- View: `entidade_acao_view()` ou `entidade_acao_cb()` (callback)

**Structs:**
- CamelCase: `Cliente`, `Evento`, `Recurso`
- Campos em minúsculas: `id`, `nome`, `cpf_cnpj`

### Comentários

```c
/* =============================================================================
 * SEÇÃO PRINCIPAL
 * ============================================================================= */

/* Função auxiliar que faz X */
void funcao_auxiliar() {
    // Comentário de linha
}
```

### Organização de Arquivo .c

```c
/* Includes */
#include "header.h"

/* Variáveis globais */
static int contador = 0;

/* =============================================================================
 * FUNÇÕES AUXILIARES
 * ============================================================================= */

/* Função auxiliar 1 */
void aux1() { ... }

/* =============================================================================
 * FUNÇÕES PRINCIPAIS
 * ============================================================================= */

/* Função principal */
void funcao_principal() { ... }

/* =============================================================================
 * CALLBACKS (apenas em views)
 * ============================================================================= */

/* Callback de botão */
int botao_cb(Ihandle* ih) { ... }
```

---

## Conceitos Importantes

### Padrão MVC

**Model:** Apenas dados e persistência. Não sabe nada de interface.

**Controller:** Lógica de negócio. Conecta Model e View.

**View:** Apenas interface. Chama o Controller para tudo.

### Fluxo de Dados

```
Usuário clica botão
    ↓
View captura clique (callback)
    ↓
View chama Controller
    ↓
Controller valida dados
    ↓
Controller chama Model
    ↓
Model persiste dados
    ↓
Controller retorna resultado
    ↓
View atualiza interface
```

### Persistência

O sistema usa ponteiros de função para abstrair a persistência:

```c
// Interface genérica
void* (*criar)(void* dados);
void* (*ler)(int id);
void (*atualizar)(void* dados);
void (*deletar)(int id);

// Implementações
pers_texto_criar()    // Salva em .txt
pers_memoria_criar()  // Salva em array
pers_binario_criar()  // Salva em .bin
```

Escolha em `src/config.h`:
```c
#define TIPO_PERSISTENCIA 1  // 1, 2 ou 3
```

---

## Como Estender o Sistema

### Adicionar Nova Entidade

1. Criar struct em `src/model/nova_entidade.h`
2. Criar funções CRUD em `src/model/nova_entidade.c`
3. Criar controller em `src/controller/nova_entidade_controller.c/h`
4. Criar view em `src/view/nova_entidade_view.c/h`
5. Adicionar no `makefile`
6. Adicionar no menu principal (`main.c`)

### Adicionar Novo Relatório

1. Criar função no controller correspondente
2. Criar função de view que chama o controller
3. Adicionar botão ou item de menu

### Adicionar Novo Tipo de Persistência

1. Criar `src/model/pers_novo_tipo.c/h`
2. Implementar funções: `criar`, `ler`, `atualizar`, `deletar`, `listar`
3. Adicionar em `src/model/pers.c` na função de inicialização
4. Definir novo número em `src/config.h`

---

## Bibliotecas Utilizadas

### IUP (Interface)
- Documentação: https://www.tecgraf.puc-rio.br/iup/
- Portable GUI toolkit
- Widgets: dialogs, buttons, text, matrix, etc.

### CD (Canvas Draw)
- Documentação: https://www.tecgraf.puc-rio.br/cd/
- Para desenhar gráficos (se necessário)
- Já linkado no projeto

### Stdlib (Padrão C)
- stdio.h - I/O
- stdlib.h - malloc, free, atoi, etc.
- string.h - strcmp, strcpy, etc.
- time.h - datas

---

## Ferramentas de Desenvolvimento

### Compilador
- MinGW GCC 8.1.0+
- C99 standard

### Make
- GNU Make
- Automação de build

### Debugger
- GDB (GNU Debugger)
- Incluído no MinGW

### Editor
- Qualquer editor de texto
- Recomendado: VS Code, Code::Blocks, Dev-C++

---

## Referências

### Linguagem C
- The C Programming Language (K&R)
- https://en.cppreference.com/w/c

### IUP
- Manual oficial: https://www.tecgraf.puc-rio.br/iup/
- Tutorial: https://www.tecgraf.puc-rio.br/iup/en/tutorial/

### Padrão MVC
- Conceito: https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller
- Aplicação em C: veja o código deste projeto

---

## Próximos Passos

1. Leia README.md para entender o projeto
2. Leia COMPILACAO.md e compile
3. Execute e explore o sistema
4. Leia EXEMPLOS.md e teste as funcionalidades
5. Leia a documentação técnica (model, controller, view)
6. Estude o código fonte
7. Faça modificações e experimente

---

## Suporte

Este é um projeto acadêmico. Documentação completa está neste repositório.

Para dúvidas:
1. Consulte esta documentação
2. Leia os comentários no código
3. Teste e experimente
