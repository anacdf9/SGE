# Documentação - Camada View

Interface gráfica com IUP

---

## Visão Geral

A camada View é responsável por:
- Criar e exibir telas (dialogs)
- Capturar entrada do usuário
- Exibir dados em tabelas (matrizes)
- Chamar o Controller para processar dados
- Atualizar interface com resultados

**Não contém:** lógica de negócio, validações complexas, persistência

---

## Biblioteca IUP

### O que é IUP?

Portable User Interface - biblioteca para criar interfaces gráficas em C.

**Site oficial:** https://www.tecgraf.puc-rio.br/iup/

**Características:**
- Multiplataforma (Windows, Linux, Mac)
- API simples em C
- Widgets nativos do sistema
- Leve e rápida

### Principais Widgets Usados

**IupDialog** - Janela principal
**IupButton** - Botão
**IupLabel** - Texto estático
**IupText** - Campo de texto
**IupList** - Lista/combo
**IupMatrix** - Tabela
**IupHbox** - Layout horizontal
**IupVbox** - Layout vertical
**IupFrame** - Moldura com título
**IupTabs** - Abas

---

## Estrutura de uma View

### Padrão Típico

```c

#include <iup.h>
#include "entidade_view.h"
#include "../controller/entidade_controller.h"
#include "ui_common.h"
#include "validation.h"


static Ihandle* dlg_entidade = NULL;
static Ihandle* txt_nome = NULL;
static Ihandle* txt_campo = NULL;
static Ihandle* matrix_lista = NULL;



void entidade_limpar_campos() {
    IupSetAttribute(txt_nome, "VALUE", "");
    IupSetAttribute(txt_campo, "VALUE", "");
}

void entidade_carregar_dados(int id) {
    Entidade* e = entidade_ler(id);
    if (e == NULL) return;
    
    IupSetAttribute(txt_nome, "VALUE", e->nome);
    IupSetAttribute(txt_campo, "VALUE", e->campo);
    
    free(e);
}



void entidade_atualizar_matriz() {
    int total;
    Entidade* lista = entidade_listar(&total);
    
    // Configurar número de linhas
    IupSetInt(matrix_lista, "NUMLIN", total);
    
    // Preencher dados
    for (int i = 0; i < total; i++) {
        IupSetAttributeId(matrix_lista, "0", i+1, (i+1));  // Número da linha
        IupSetAttributeId(matrix_lista, "", i+1, lista[i].nome);
        IupSetAttributeId(matrix_lista, "", i+1, lista[i].campo);
    }
    
    IupSetAttribute(matrix_lista, "REDRAW", "ALL");
    free(lista);
}



int entidade_salvar_cb(Ihandle* ih) {
    // Obter valores dos campos
    char* nome = IupGetAttribute(txt_nome, "VALUE");
    char* campo = IupGetAttribute(txt_campo, "VALUE");
    
    // Validar
    if (strlen(nome) == 0) {
        IupMessage("Erro", "Nome obrigatório");
        return IUP_DEFAULT;
    }
    
    // Criar struct
    Entidade e;
    strcpy(e.nome, nome);
    strcpy(e.campo, campo);
    
    // Chamar controller
    int resultado = entidade_salvar_controller(&e);
    
    if (resultado == SUCESSO) {
        IupMessage("Sucesso", "Registro salvo");
        entidade_limpar_campos();
        entidade_atualizar_matriz();
    } else {
        IupMessage("Erro", "Erro ao salvar");
    }
    
    return IUP_DEFAULT;
}

int entidade_cancelar_cb(Ihandle* ih) {
    entidade_limpar_campos();
    return IUP_DEFAULT;
}

int entidade_excluir_cb(Ihandle* ih) {
    // Obter linha selecionada
    char* linha_str = IupGetAttribute(matrix_lista, "VALUE");
    int linha = atoi(linha_str);
    
    if (linha == 0) {
        IupMessage("Aviso", "Selecione um registro");
        return IUP_DEFAULT;
    }
    
    // Confirmar exclusão
    int resposta = IupAlarm("Confirmar", "Deseja excluir?", "Sim", "Não", NULL);
    if (resposta != 1) return IUP_DEFAULT;
    
    // Obter ID da linha
    char* id_str = IupGetAttributeId(matrix_lista, "", linha);
    int id = atoi(id_str);
    
    // Chamar controller
    int resultado = entidade_excluir_controller(id);
    
    if (resultado == SUCESSO) {
        IupMessage("Sucesso", "Registro excluído");
        entidade_atualizar_matriz();
    } else {
        IupMessage("Erro", "Erro ao excluir");
    }
    
    return IUP_DEFAULT;
}



Ihandle* entidade_criar_formulario() {
    // Labels
    Ihandle* lbl_nome = IupLabel("Nome:");
    Ihandle* lbl_campo = IupLabel("Campo:");
    
    // Campos de texto
    txt_nome = IupText(NULL);
    IupSetAttribute(txt_nome, "SIZE", "200x");
    
    txt_campo = IupText(NULL);
    IupSetAttribute(txt_campo, "SIZE", "200x");
    
    // Botões
    Ihandle* btn_salvar = IupButton("Salvar", NULL);
    IupSetCallback(btn_salvar, "ACTION", (Icallback)entidade_salvar_cb);
    
    Ihandle* btn_cancelar = IupButton("Cancelar", NULL);
    IupSetCallback(btn_cancelar, "ACTION", (Icallback)entidade_cancelar_cb);
    
    // Layout
    Ihandle* grid = IupGridBox(
        lbl_nome, txt_nome,
        lbl_campo, txt_campo,
        NULL
    );
    IupSetAttribute(grid, "NUMDIV", "2");
    IupSetAttribute(grid, "GAPLIN", "5");
    IupSetAttribute(grid, "GAPCOL", "10");
    
    Ihandle* btns = IupHbox(
        btn_salvar,
        btn_cancelar,
        NULL
    );
    IupSetAttribute(btns, "GAP", "5");
    
    Ihandle* vbox = IupVbox(
        grid,
        btns,
        NULL
    );
    IupSetAttribute(vbox, "GAP", "10");
    IupSetAttribute(vbox, "MARGIN", "10x10");
    
    return vbox;
}

Ihandle* entidade_criar_lista() {
    // Matriz
    matrix_lista = IupMatrix(NULL);
    IupSetAttribute(matrix_lista, "NUMCOL", "3");
    IupSetAttribute(matrix_lista, "NUMLIN", "0");
    
    // Títulos das colunas
    IupSetAttribute(matrix_lista, "0:0", "Nr");
    IupSetAttribute(matrix_lista, "0:1", "Nome");
    IupSetAttribute(matrix_lista, "0:2", "Campo");
    
    // Largura das colunas
    IupSetAttribute(matrix_lista, "WIDTH0", "30");
    IupSetAttribute(matrix_lista, "WIDTH1", "200");
    IupSetAttribute(matrix_lista, "WIDTH2", "150");
    
    // Configurações
    IupSetAttribute(matrix_lista, "READONLY", "YES");
    IupSetAttribute(matrix_lista, "RESIZEMATRIX", "YES");
    IupSetAttribute(matrix_lista, "MARKMODE", "LIN");
    
    // Botões
    Ihandle* btn_excluir = IupButton("Excluir", NULL);
    IupSetCallback(btn_excluir, "ACTION", (Icallback)entidade_excluir_cb);
    
    Ihandle* btn_atualizar = IupButton("Atualizar", NULL);
    IupSetCallback(btn_atualizar, "ACTION", (Icallback)entidade_atualizar_matriz);
    
    Ihandle* btns = IupHbox(
        btn_excluir,
        btn_atualizar,
        NULL
    );
    IupSetAttribute(btns, "GAP", "5");
    
    Ihandle* vbox = IupVbox(
        matrix_lista,
        btns,
        NULL
    );
    IupSetAttribute(vbox, "GAP", "10");
    IupSetAttribute(vbox, "MARGIN", "10x10");
    
    return vbox;
}


void entidade_view_show() {
    if (dlg_entidade == NULL) {
        // Criar abas
        Ihandle* formulario = entidade_criar_formulario();
        Ihandle* lista = entidade_criar_lista();
        
        Ihandle* tabs = IupTabs(
            IupSetAttributes(IupVbox(formulario, NULL), "TABTITLE=Cadastro"),
            IupSetAttributes(IupVbox(lista, NULL), "TABTITLE=Lista"),
            NULL
        );
        
        // Criar dialog
        dlg_entidade = IupDialog(tabs);
        IupSetAttribute(dlg_entidade, "TITLE", "Gerenciar Entidades");
        IupSetAttribute(dlg_entidade, "SIZE", "600x400");
    }
    
    // Carregar dados
    entidade_atualizar_matriz();
    
    // Exibir
    IupShow(dlg_entidade);
}
```

---

## IupMatrix - Tabelas

### Criar Matriz

```c
Ihandle* matrix = IupMatrix(NULL);
IupSetAttribute(matrix, "NUMCOL", "3");      // 3 colunas
IupSetAttribute(matrix, "NUMLIN", "10");     // 10 linhas
```

### Configurar Colunas

```c
// Títulos (linha 0)
IupSetAttribute(matrix, "0:0", "ID");
IupSetAttribute(matrix, "0:1", "Nome");
IupSetAttribute(matrix, "0:2", "Valor");

// Larguras
IupSetAttribute(matrix, "WIDTH0", "50");
IupSetAttribute(matrix, "WIDTH1", "200");
IupSetAttribute(matrix, "WIDTH2", "100");
```

### Preencher Dados

```c
// Célula [linha:coluna]
IupSetAttributeId(matrix, "", 1, "João");    // Linha 1, col 1
IupSetAttributeId(matrix, "", 1, "100.00");  // Linha 1, col 2

// Com loop
for (int i = 0; i < total; i++) {
    char buffer[20];
    sprintf(buffer, "%d", lista[i].id);
    IupSetAttributeId(matrix, "", i+1, buffer);
    IupSetAttributeId(matrix, "", i+1, lista[i].nome);
}

// Redesenhar
IupSetAttribute(matrix, "REDRAW", "ALL");
```

### Obter Seleção

```c
char* linha_str = IupGetAttribute(matrix, "VALUE");
int linha = atoi(linha_str);

if (linha > 0) {
    char* id_str = IupGetAttributeId(matrix, "", linha);
    int id = atoi(id_str);
}
```

### Atributos Úteis

```c
IupSetAttribute(matrix, "READONLY", "YES");         // Somente leitura
IupSetAttribute(matrix, "RESIZEMATRIX", "YES");     // Redimensionável
IupSetAttribute(matrix, "MARKMODE", "LIN");         // Selecionar linha
IupSetAttribute(matrix, "HIDEFOCUS", "YES");        // Sem foco visual
```

---

## Layouts

### IupHbox - Horizontal

```c
Ihandle* hbox = IupHbox(
    widget1,
    widget2,
    widget3,
    NULL  // Sempre terminar com NULL
);
IupSetAttribute(hbox, "GAP", "10");  // Espaço entre widgets
```

### IupVbox - Vertical

```c
Ihandle* vbox = IupVbox(
    widget1,
    widget2,
    widget3,
    NULL
);
IupSetAttribute(vbox, "GAP", "5");
IupSetAttribute(vbox, "MARGIN", "10x10");  // Margem externa
```

### IupGridBox - Grade

```c
Ihandle* grid = IupGridBox(
    label1, campo1,
    label2, campo2,
    label3, campo3,
    NULL
);
IupSetAttribute(grid, "NUMDIV", "2");      // 2 colunas
IupSetAttribute(grid, "GAPLIN", "5");      // Espaço entre linhas
IupSetAttribute(grid, "GAPCOL", "10");     // Espaço entre colunas
IupSetAttribute(grid, "ALIGNMENTLIN", "ACENTER");  // Alinhar verticalmente
```

---

## Widgets Comuns

### Botão

```c
Ihandle* btn = IupButton("Texto", NULL);
IupSetCallback(btn, "ACTION", (Icallback)minha_funcao_cb);
IupSetAttribute(btn, "SIZE", "80x");  // Largura 80 pixels
```

### Campo de Texto

```c
Ihandle* txt = IupText(NULL);
IupSetAttribute(txt, "SIZE", "200x");
IupSetAttribute(txt, "MASK", "/d*");  // Apenas números

// Obter valor
char* valor = IupGetAttribute(txt, "VALUE");

// Definir valor
IupSetAttribute(txt, "VALUE", "texto");
```

### Label

```c
Ihandle* lbl = IupLabel("Texto:");
IupSetAttribute(lbl, "SIZE", "80x");
```

### Lista (Dropdown)

```c
Ihandle* list = IupList(NULL);
IupSetAttribute(list, "DROPDOWN", "YES");
IupSetAttribute(list, "1", "Opção 1");
IupSetAttribute(list, "2", "Opção 2");
IupSetAttribute(list, "3", "Opção 3");
IupSetAttribute(list, "VALUE", "1");  // Selecionar primeira

// Obter seleção
char* sel = IupGetAttribute(list, "VALUE");
int indice = atoi(sel);
```

### Frame

```c
Ihandle* frame = IupFrame(
    IupVbox(
        widget1,
        widget2,
        NULL
    )
);
IupSetAttribute(frame, "TITLE", "Título do Frame");
```

---

## Callbacks

### O que é Callback?

Função chamada quando ocorre um evento (clique, mudança de valor, etc).

### Sintaxe

```c
int minha_funcao_cb(Ihandle* ih) {
    // Processar evento
    return IUP_DEFAULT;  // ou IUP_CLOSE para fechar dialog
}

// Registrar callback
IupSetCallback(widget, "ACTION", (Icallback)minha_funcao_cb);
```

### Retornos

- `IUP_DEFAULT` - Continuar normalmente
- `IUP_CLOSE` - Fechar dialog
- `IUP_IGNORE` - Ignorar ação padrão

### Callbacks Comuns

**ACTION** - Botão clicado, Enter pressionado
**VALUECHANGED_CB** - Valor do campo mudou
**SELECTION_CB** - Seleção na lista mudou
**CLICK_CB** - Clique na matriz

---

## Diálogos

### IupDialog

```c
Ihandle* dlg = IupDialog(conteudo);
IupSetAttribute(dlg, "TITLE", "Título");
IupSetAttribute(dlg, "SIZE", "400x300");
IupShow(dlg);
```

### IupMessage

```c
IupMessage("Título", "Mensagem");
```

### IupAlarm

```c
int resposta = IupAlarm("Título", "Pergunta?", "Sim", "Não", NULL);
if (resposta == 1) {
    // Sim
} else {
    // Não
}
```

### IupGetParam

```c
char nome[100] = "";
int idade = 0;
float altura = 0.0;

int ok = IupGetParam("Dados", NULL, NULL,
    "Nome: %s\n"
    "Idade: %i\n"
    "Altura: %r\n",
    nome, &idade, &altura, NULL);

if (ok) {
    // Usuário clicou OK
}
```

---

## Validação de Entrada

### validation.c/h

Funções auxiliares para validar entrada:

```c
int validar_cpf_input(char* cpf);
int validar_email_input(char* email);
int validar_data_input(char* data);
int validar_numero_input(char* numero);
```

Uso:

```c
char* cpf = IupGetAttribute(txt_cpf, "VALUE");
if (!validar_cpf_input(cpf)) {
    IupMessage("Erro", "CPF inválido");
    IupSetFocus(txt_cpf);
    return IUP_DEFAULT;
}
```

---

## Funções Comuns (ui_common.c/h)

### Criar Botões Padrão

```c
Ihandle* criar_botoes_padrao(Icallback salvar_cb, Icallback cancelar_cb) {
    Ihandle* btn_salvar = IupButton("Salvar", NULL);
    IupSetCallback(btn_salvar, "ACTION", salvar_cb);
    
    Ihandle* btn_cancelar = IupButton("Cancelar", NULL);
    IupSetCallback(btn_cancelar, "ACTION", cancelar_cb);
    
    return IupHbox(btn_salvar, btn_cancelar, NULL);
}
```

### Formatar Moeda

```c
void formatar_moeda(float valor, char* buffer) {
    sprintf(buffer, "R$ %.2f", valor);
}
```

### Preencher Combo

```c
void preencher_combo_clientes(Ihandle* combo) {
    int total;
    Cliente* lista = cliente_listar(&total);
    
    for (int i = 0; i < total; i++) {
        char buffer[10];
        sprintf(buffer, "%d", i+1);
        IupSetAttribute(combo, buffer, lista[i].nome);
    }
    
    free(lista);
}
```

---

## Padrão de Abas (Tabs)

```c
Ihandle* aba1 = IupVbox(/* conteúdo */, NULL);
IupSetAttribute(aba1, "TABTITLE", "Cadastro");

Ihandle* aba2 = IupVbox(/* conteúdo */, NULL);
IupSetAttribute(aba2, "TABTITLE", "Lista");

Ihandle* tabs = IupTabs(aba1, aba2, NULL);

Ihandle* dlg = IupDialog(tabs);
```

---

## Atualizar Interface

### Atualizar Valor de Campo

```c
IupSetAttribute(txt_campo, "VALUE", "novo valor");
```

### Redesenhar Widget

```c
IupSetAttribute(widget, "REDRAW", "ALL");
```

### Dar Foco

```c
IupSetFocus(txt_campo);
```

### Desabilitar Widget

```c
IupSetAttribute(widget, "ACTIVE", "NO");
```

---

## Máscaras de Entrada

```c
// Apenas números
IupSetAttribute(txt, "MASK", "/d*");

// CPF
IupSetAttribute(txt, "MASK", "/d/d/d./d/d/d./d/d/d-/d/d");

// CNPJ
IupSetAttribute(txt, "MASK", "/d/d./d/d/d./d/d/d//d/d/d/d-/d/d");

// Data
IupSetAttribute(txt, "MASK", "/d/d//d/d//d/d/d/d");

// Telefone
IupSetAttribute(txt, "MASK", "(/d/d) /d/d/d/d/d-/d/d/d/d");
```

---

## Boas Práticas

1. **Variáveis globais para widgets principais:**
```c
static Ihandle* dlg_principal = NULL;
static Ihandle* matrix_dados = NULL;
```

2. **Sempre validar entrada:**
```c
if (strlen(valor) == 0) {
    IupMessage("Erro", "Campo obrigatório");
    return IUP_DEFAULT;
}
```

3. **Chamar Controller para lógica:**
```c
// Não fazer validação complexa na View
int resultado = controller_validar(dados);
if (resultado != SUCESSO) {
    IupMessage("Erro", "Dados inválidos");
}
```

4. **Liberar memória:**
```c
Cliente* c = cliente_ler(id);
// usar c...
free(c);
```

5. **Atualizar matriz após mudanças:**
```c
entidade_atualizar_matriz();
```

---

## Exemplo Completo Simples

```c
#include <iup.h>
#include <stdlib.h>

int btn_ok_cb(Ihandle* ih) {
    Ihandle* txt = (Ihandle*)IupGetAttribute(ih, "TXT");
    char* nome = IupGetAttribute(txt, "VALUE");
    
    IupMessagef("Olá", "Olá, %s!", nome);
    
    return IUP_DEFAULT;
}

int main(int argc, char** argv) {
    IupOpen(&argc, &argv);
    
    Ihandle* txt_nome = IupText(NULL);
    IupSetAttribute(txt_nome, "SIZE", "200x");
    
    Ihandle* btn_ok = IupButton("OK", NULL);
    IupSetCallback(btn_ok, "ACTION", (Icallback)btn_ok_cb);
    IupSetAttribute(btn_ok, "TXT", (char*)txt_nome);
    
    Ihandle* vbox = IupVbox(
        IupLabel("Nome:"),
        txt_nome,
        btn_ok,
        NULL
    );
    IupSetAttribute(vbox, "MARGIN", "10x10");
    IupSetAttribute(vbox, "GAP", "5");
    
    Ihandle* dlg = IupDialog(vbox);
    IupSetAttribute(dlg, "TITLE", "Exemplo");
    IupSetAttribute(dlg, "SIZE", "300x150");
    
    IupShow(dlg);
    IupMainLoop();
    IupClose();
    
    return 0;
}
```
---

## Views Especiais

### feedback_view.c/h

**Propósito:**
Interface gráfica para geração de relatórios com filtros avançados, visualização em tela e exportação CSV.

**Componentes Principais:**
```c
// Seleção de tipo de relatório
Ihandle* lst_relatorio;     // List com 7 opções

// Painéis dinâmicos de filtro (aparecem/desaparecem)
Ihandle* pnl_filtro_cliente;
Ihandle* pnl_filtro_evento;
Ihandle* pnl_filtro_recurso;
// ... para cada tipo de relatório

// Exibição de resultados
Ihandle* txt_resultado;     // Text com conteúdo formatado

// Ações
Ihandle* btn_gerar;         // Botão para gerar relatório
Ihandle* btn_exportar_csv;  // Botão para exportar
```

**Callbacks Principais:**
```c
int feedback_selecao_relatorio_cb(Ihandle* ih);  // Alterna painéis de filtro
int feedback_gerar_cb(Ihandle* ih);              // Gera e exibe relatório
int feedback_exportar_csv_cb(Ihandle* ih);       // Abre diálogo de arquivo
```

**Funcionalidades:**
- List dropdown para seleção do tipo de relatório
- Painéis dinâmicos com filtros específicos para cada relatório
- Text widget com scroll para visualização de resultados formatados
- File dialog para escolher destino do CSV
- Validações de entrada (datas, ranges)
- Formatação automática com colunas alinhadas

### trade_view.c/h

**Propósito:**
Interface gráfica para importar/exportar dados em formato XML com seleção seletiva de tabelas.

**Componentes Principais:**
```c
// Modo de operação
Ihandle* rd_exportar;       // Radio para modo exportação
Ihandle* rd_importar;       // Radio para modo importação

// Seleção de tabelas (checkboxes)
Ihandle* chk_clientes;      // Flag TRADE_TABLE_CLIENTES
Ihandle* chk_recursos;      // Flag TRADE_TABLE_RECURSOS
Ihandle* chk_eventos;       // Flag TRADE_TABLE_EVENTOS
// ... 7 checkboxes adicionais

// Botões auxiliares
Ihandle* btn_selecionar_todas;    // Marca todas as caixas
Ihandle* btn_desselecionar_todas; // Desmarca todas as caixas

// Opção para importação
Ihandle* chk_sobrescrever;  // Marca para substituir dados existentes

// Arquivo
Ihandle* txt_arquivo;       // Path do arquivo XML
Ihandle* btn_escolher;      // File dialog para seleção

// Ações
Ihandle* btn_processar;     // Exportar ou Importar
```

**Callbacks Principais:**
```c
int trade_modo_cb(Ihandle* ih);           // Alterna entre export/import
int trade_sel_todas_cb(Ihandle* ih);      // Seleciona todas as tabelas
int trade_desel_todas_cb(Ihandle* ih);    // Desseleciona todas
int trade_escolher_arquivo_cb(Ihandle* ih); // File dialog
int trade_processar_cb(Ihandle* ih);      // Executar export/import
```

**Funcionalidades:**
- Toggle entre modo exportação e importação
- 10 checkboxes para seleção de tabelas (com flags bitwise)
- Botões de atalho para select-all e deselect-all
- File dialog para escolher arquivo XML
- Checkbox para opção de sobrescrita na importação
- Validações antes de processar
- Mensagens de progresso e confirmação
- Tratamento de erros com diálogos informativos

---

## Arquivos da View

```
src/view/
├── cliente_view.c/h
├── recurso_view.c/h
├── evento_view.c/h
├── equipe_view.c/h
├── fornecedor_view.c/h
├── produtora_view.c/h
├── operador_view.c/h
├── alocacao_view.c/h
├── transacoes_view.c/h
├── feedback_view.c/h       # Relatórios e análises
├── trade_view.c/h          # Importação/Exportação XML
├── ui_common.c/h           # Funções auxiliares de UI
└── validation.c/h          # Validações de entrada
```

---

## Documentação IUP

- Manual: https://www.tecgraf.puc-rio.br/iup/en/
- Tutoriais: https://www.tecgraf.puc-rio.br/iup/en/tutorial/
- Referência de Widgets: https://www.tecgraf.puc-rio.br/iup/en/ctrl/

---

## Veja Também

- `src/controller/README.md` - Lógica de negócio
- `src/model/README.md` - Estruturas de dados
- `DOCUMENTACAO.md` - Índice completo
