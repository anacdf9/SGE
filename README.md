# SGE - Sistema de Gestão de Eventos


Sistema completo desenvolvido em C para gerenciamento de eventos, incluindo cadastros, orçamentos e controle financeiro.

---

## Sobre o Projeto

O SGE (Sistema de Gestão de Eventos) permite gerenciar todos os aspectos de uma produtora de eventos, desde cadastros básicos até o controle financeiro completo.

### Funcionalidades Principais

- Cadastro de clientes, fornecedores, equipes e recursos
- Criação e gerenciamento de orçamentos de eventos
- Controle financeiro (caixa, contas a receber e pagar)
- Cálculo automático de preços de locação
- Geração de relatórios

---

## Arquitetura

O sistema utiliza o padrão **MVC (Model-View-Controller)**:

```
SGE/
├── src/
│   ├── model/          # Estruturas de dados e persistência
│   ├── controller/     # Lógica de negócio
│   └── view/           # Interface gráfica (IUP)
├── lib/                # Bibliotecas (IUP)
└── main.c              # Programa principal
```

### Organização das Camadas

**Model (Dados)**
- Estruturas de dados (structs)
- Sistema de persistência (texto, memória, binário)
- Arquivos: `pers.c`, `cliente.c`, `evento.c`, etc.

**Controller (Lógica)**
- Regras de negócio
- Validações
- Cálculos
- Arquivos: `cliente_controller.c`, `evento_controller.c`, etc.

**View (Interface)**
- Telas e formulários
- Matrizes (tabelas)
- Callbacks
- Arquivos: `cliente_view.c`, `evento_view.c`, etc.

---

## Tecnologias

- **Linguagem:** C99
- **Interface:** IUP (Portable User Interface)
- **Compilador:** MinGW GCC
- **Build:** GNU Make

---

## Sistema de Persistência

O sistema oferece 3 formas de armazenamento de dados (configurável em `src/config.h`):

### Modo 1: Arquivos Texto (CSV)
- Formato: campos separados por `;`
- Vantagem: fácil de visualizar e editar
- Arquivos: `clientes.txt`, `eventos.txt`, etc.

### Modo 2: Memória (RAM)
- Formato: arrays estáticos
- Vantagem: muito rápido
- Desvantagem: dados perdidos ao fechar

### Modo 3: Arquivos Binários
- Formato: binário (fwrite/fread)
- Vantagem: compacto e eficiente
- Arquivos: `clientes.bin`, `eventos.bin`, etc.

---

## Compilação

### Pré-requisitos
- MinGW instalado e no PATH
- Biblioteca IUP (já incluída no projeto)

### Comandos

```bash
# Limpar e compilar
mingw32-make clean
mingw32-make

# Executar
./SGE.exe
```

Veja mais detalhes em `COMPILACAO.md`

---

## Módulos do Sistema

### 1. Cadastros
- Produtora (dados da empresa + margem de lucro)
- Clientes (pessoa física ou jurídica)
- Equipe Interna (funcionários + valor/hora)
- Recursos/Equipamentos (estoque + preços)
- Fornecedores/Parceiros (serviços terceirizados)
- Operadores (usuários do sistema)

### 2. Orçamentos e Eventos
- Criação de orçamentos
- Alocação de recursos
- Alocação de equipe (horas trabalhadas)
- Contratação de fornecedores
- Cálculo automático de valores
- Aprovação e finalização de eventos

### 3. Transações Financeiras

**Caixa**
- Lançamentos de entrada e saída
- Cálculo automático de saldo

**Contas a Receber**
- Faturamento de eventos
- Controle de recebimentos parciais
- Status: Aberto / Parcial / Liquidado

**Compras**
- Registro de compras de equipamentos
- Cálculo automático do preço de locação
- Parcelamento (à vista ou prazo)

**Contas a Pagar**
- Controle de parcelas
- Baixa de pagamentos

---

## Cálculo de Preço de Locação

O sistema calcula automaticamente o preço de locação baseado em:

```
Preço de Locação = Custo Unitário 
                 + (Frete Total / Quantidade)
                 + (Imposto Total / Quantidade)
                 + (Margem de Lucro × Custo Unitário)
```

**Exemplo:**
```
Compra de 10 microfones:
- Custo: R$ 100,00/un
- Frete total: R$ 50,00 → R$ 5,00/un
- Imposto total: R$ 30,00 → R$ 3,00/un
- Margem: 10% → R$ 10,00/un

Preço de Locação = R$ 118,00/un
```

A margem de lucro é configurada no cadastro da Produtora.

---

## Arquivos de Dados

Os dados são salvos na raiz do projeto:

```
clientes.txt
recursos.txt
eventos.txt
caixa.txt
compras.txt
compras_itens.txt
contas_receber.txt
contas_pagar.txt
produtora.txt
equipes.txt
fornecedores.txt
operadores.txt
```

---

## Documentação

- `README.md` - Este arquivo (visão geral)
- `COMPILACAO.md` - Como compilar e resolver problemas
- `EXEMPLOS.md` - Exemplos práticos de uso
- `DOCUMENTACAO.md` - Índice de toda documentação
- `src/model/README.md` - Documentação da camada Model
- `src/controller/README.md` - Documentação da camada Controller
- `src/view/README.md` - Documentação da camada View

---

## Conceitos Aplicados

Este projeto demonstra conhecimentos em:

- Estruturas de dados (structs, arrays)
- Manipulação de arquivos (texto e binário)
- Alocação dinâmica de memória
- Ponteiros e referências
- Modularização de código
- Padrão MVC
- Interface gráfica
- Algoritmos de busca
- Validações e tratamento de erros
- Cálculos financeiros

---

## Estrutura de Arquivos Completa

```
SGE/
├── main.c
├── makefile
├── README.md
├── COMPILACAO.md
├── EXEMPLOS.md
├── DOCUMENTACAO.md
├── lib/
│   ├── iup64/
│   └── cd64/
└── src/
    ├── config.h
    ├── model/
    │   ├── pers.c/h
    │   ├── pers_texto.c/h
    │   ├── pers_memoria.c/h
    │   ├── pers_binario.c/h
    │   ├── cliente.c/h
    │   ├── evento.c/h
    │   ├── recurso.c/h
    │   └── transacoes.h
    ├── controller/
    │   ├── cliente_controller.c/h
    │   ├── evento_controller.c/h
    │   ├── recurso_controller.c/h
    │   └── transacoes_controller.c/h
    └── view/
        ├── cliente_view.c/h
        ├── evento_view.c/h
        ├── recurso_view.c/h
        ├── transacoes_view.c/h
        ├── ui_common.c/h
        └── validation.c/h
```

---

## Como Usar

1. Compile o projeto: `mingw32-make`
2. Execute: `./SGE.exe`
3. Configure a Produtora (margem de lucro)
4. Cadastre clientes, recursos, etc.
5. Crie eventos e faça orçamentos
6. Gerencie as finanças

Veja exemplos detalhados em `EXEMPLOS.md`

---

## Resolução de Problemas

Problemas comuns e soluções estão documentados em `COMPILACAO.md`

---

## Autor

Ana Clara Silveira Ferreira

---

## Licença

Projeto acadêmico - fins educacionais
