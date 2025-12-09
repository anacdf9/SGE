# Exemplos de Uso

Passo a passo de como usar o SGE

---

## Primeiro Uso

### 1. Configurar a Produtora

Ao abrir o sistema pela primeira vez:
1. Menu Cadastros > Produtora
2. Preencha:
   - Nome: "EventosPro"
   - CNPJ: "12.345.678/0001-90"
   - Telefone: "(11) 98765-4321"
   - Margem de Lucro: 10 (significa 10%)
3. Salvar

A margem de lucro será usada no cálculo automático do preço de locação.

---

## Cadastros Básicos

### Cadastrar Cliente Pessoa Física

1. Menu Cadastros > Clientes > Novo
2. Preencha:
   - Nome: "João Silva"
   - CPF/CNPJ: "123.456.789-00"
   - Telefone: "(11) 91234-5678"
   - Email: "joao@email.com"
3. Salvar

### Cadastrar Cliente Pessoa Jurídica

1. Menu Cadastros > Clientes > Novo
2. Preencha:
   - Nome: "Empresa XYZ Ltda"
   - CPF/CNPJ: "12.345.678/0001-99"
   - Telefone: "(11) 3456-7890"
   - Email: "contato@xyz.com.br"
3. Salvar

### Cadastrar Recurso (Equipamento)

1. Menu Cadastros > Recursos > Novo
2. Preencha:
   - Nome: "Microfone Shure SM58"
   - Quantidade: 10
   - Preço Locação: 50.00
   - Descrição: "Microfone dinâmico cardióide"
3. Salvar

### Cadastrar Equipe (Funcionário)

1. Menu Cadastros > Equipe Interna > Novo
2. Preencha:
   - Nome: "Carlos Técnico"
   - Função: "Técnico de Som"
   - Valor/Hora: 80.00
   - Telefone: "(11) 94567-8901"
3. Salvar

### Cadastrar Fornecedor

1. Menu Cadastros > Fornecedores > Novo
2. Preencha:
   - Nome: "SomPro Equipamentos"
   - CNPJ: "98.765.432/0001-10"
   - Tipo Serviço: "Aluguel de som profissional"
   - Telefone: "(11) 3333-4444"
3. Salvar

---

## Criar um Evento

### Passo a Passo

1. Menu Orçamentos > Novo Evento
2. Preencha dados básicos:
   - Nome: "Casamento João e Maria"
   - Cliente: Selecione "João Silva"
   - Data Início: 15/12/2024
   - Data Fim: 15/12/2024
   - Hora Início: 19:00
   - Hora Fim: 23:00
   - Local: "Chácara Flor do Campo"
   - Descrição: "Casamento com 150 convidados"
3. Salvar

### Adicionar Recursos ao Evento

1. Abra o evento criado
2. Aba "Recursos"
3. Adicionar Item:
   - Recurso: "Microfone Shure SM58"
   - Quantidade: 2
   - Valor Unitário: 50.00
4. Adicionar mais itens conforme necessário
5. Salvar

### Adicionar Equipe ao Evento

1. Aba "Equipe"
2. Adicionar Membro:
   - Funcionário: "Carlos Técnico"
   - Horas Trabalhadas: 6
   - Valor/Hora: 80.00
   - Total: 480.00 (calculado automaticamente)
3. Salvar

### Adicionar Fornecedor ao Evento

1. Aba "Fornecedores"
2. Adicionar Serviço:
   - Fornecedor: "SomPro Equipamentos"
   - Descrição: "Aluguel de mesa de som 32 canais"
   - Valor: 1500.00
3. Salvar

### Ver Valor Total do Evento

O sistema calcula automaticamente:
```
Recursos:     R$  100,00 (2 microfones × 50)
Equipe:       R$  480,00 (6 horas × 80)
Fornecedores: R$ 1500,00
---------------------------------
TOTAL:        R$ 2080,00
```

---

## Transações Financeiras

### Registrar Compra de Equipamento

1. Menu Transações > Compras > Nova Compra
2. Dados da Compra:
   - Fornecedor: "SomPro Equipamentos"
   - Data: 01/10/2024
   - Valor Total: 1050.00
   - Tipo Pagamento: "Prazo"
   - Parcelas: 3
3. Itens da Compra:
   - Recurso: "Microfone Shure SM58"
   - Quantidade: 10
   - Custo Unitário: 100.00
   - Frete Total: 50.00
   - Imposto Total: 30.00
4. Salvar

O sistema calcula automaticamente o novo preço de locação:
```
Custo: R$ 100,00
+ Frete: R$ 5,00 (50 ÷ 10)
+ Imposto: R$ 3,00 (30 ÷ 10)
+ Margem 10%: R$ 10,00
= Preço Locação: R$ 118,00
```

### Gerar Conta a Receber

1. Finalize um evento
2. Sistema pergunta se quer gerar conta a receber
3. Confirme
4. Informe:
   - Vencimento: 30/12/2024
   - Parcelas: 2 (divide o valor)

### Lançar no Caixa

1. Menu Transações > Caixa > Novo Lançamento
2. Entrada:
   - Tipo: Entrada
   - Valor: 1040.00
   - Data: 01/11/2024
   - Descrição: "Pagamento evento casamento - parcela 1/2"
3. Salvar

---

## Fluxo Completo

### Exemplo: Evento do Início ao Fim

**1. Cadastros (fazer uma vez)**
- Cliente: "Maria Santos"
- Recursos: Microfones, caixas de som, cabos
- Equipe: 2 técnicos
- Fornecedores: DJ, decoração

**2. Criar Orçamento**
- Novo evento: "Aniversário 15 anos"
- Adicionar recursos necessários
- Adicionar equipe (horas estimadas)
- Adicionar fornecedores
- Sistema mostra: Total R$ 5.000,00

**3. Cliente Aprova**
- Alterar status para "Aprovado"
- Gerar conta a receber (3 parcelas)

**4. Realizar o Evento**
- Alocar recursos
- Registrar horas reais da equipe
- Confirmar serviços de fornecedores

**5. Finalizar Evento**
- Status: "Finalizado"
- Verificar valor final

**6. Receber Pagamentos**
- Cliente paga parcela 1: lançar no caixa
- Cliente paga parcela 2: lançar no caixa
- Cliente paga parcela 3: lançar no caixa
- Conta a receber fica "Liquidada"

**7. Pagar Contas**
- Pagar fornecedores
- Registrar em contas a pagar
- Lançar saída no caixa

---

## Relatórios

### Ver Saldo do Caixa

1. Menu Transações > Caixa
2. Sistema mostra:
   - Todas entradas
   - Todas saídas
   - Saldo atual

### Ver Contas a Receber

1. Menu Transações > Contas a Receber
2. Filtrar por status:
   - Aberto (não recebeu nada)
   - Parcial (recebeu parte)
   - Liquidado (recebeu tudo)

### Ver Eventos por Período

1. Menu Orçamentos > Listar Eventos
2. Filtrar por data
3. Ver totais e status

---

## Módulo de Feedback (Relatórios)

### Gerar Relatório de Clientes

1. Menu Ferramentas > Feedback (Relatórios)
2. Selecione na lista: "Clientes"
3. Configure filtros (opcional):
   - Tipo: "Pessoa Jurídica"
   - Status: "Ativos"
   - Período: deixe em branco para todos
4. Clique "Gerar Relatório"
5. Os dados aparecem na tela, você pode:
   - **Visualizar:** Scroll na área de texto
   - **Exportar CSV:** Clique "Exportar para CSV" e escolha local do arquivo

### Gerar Relatório de Eventos

1. Menu Ferramentas > Feedback (Relatórios)
2. Selecione na lista: "Eventos"
3. Configure filtros:
   - Status: "Finalizado" (para eventos encerrados)
   - Cliente: Selecione um cliente específico ou deixe em branco
   - Valor: Mín. 1000, Máx. 5000 (para eventos nessa faixa)
   - Período: 01/01/2024 até 31/12/2024
4. Clique "Gerar Relatório"

### Relatório de Caixa (Fluxo Financeiro)

1. Menu Ferramentas > Feedback (Relatórios)
2. Selecione na lista: "Caixa"
3. Defina período desejado
4. Os dados mostram:
   - Todas as entradas e saídas
   - Saldo acumulado
   - Categorias

### Exportar para CSV (Excel)

Todos os relatórios podem ser exportados:
1. Após gerar o relatório
2. Clique em "Exportar para CSV"
3. Escolha a pasta e nome do arquivo
4. O arquivo será criado com extensão .csv
5. Abra no Excel, Google Sheets ou editor de texto

**Dica:** Use ponto-e-vírgula (;) como delimitador se importar no Excel.

---

## Módulo de Trade (Importação/Exportação XML)

### Exportar Dados

1. Menu Ferramentas > Trade (Importação/Exportação)
2. Certifique-se que "Exportar" está selecionado
3. Marque as tabelas que quer exportar:
   - ☐ Clientes
   - ☐ Recursos
   - ☐ Eventos
   - ☐ Fornecedores
   - ☐ Equipes
   - ☐ Operadores
   - ☐ Compras
   - ☐ Contas a Receber
   - ☐ Contas a Pagar
   - ☐ Caixa
4. Use "Selecionar Todas" se quiser tudo
5. Clique "Escolher Arquivo"
6. Digite nome como: `backup_2024.xml`
7. Clique "Exportar"

### Importar Dados

1. Menu Ferramentas > Trade (Importação/Exportação)
2. Selecione "Importar"
3. Marque as tabelas que quer importar
4. Opção de sobrescrita:
   - ☑ Sobrescrever: substitui dados existentes
   - ☐ Manter: mantém dados atuais, ignora duplicatas
5. Clique "Escolher Arquivo"
6. Selecione um arquivo XML exportado
7. Clique "Importar"

### Exemplo de Arquivo XML

```xml
<?xml version="1.0" encoding="UTF-8"?>
<sge>
    <clientes>
        <cliente>
            <id>1</id>
            <nome>João Silva</nome>
            <cpf_cnpj>123.456.789-00</cpf_cnpj>
            <telefone>(11) 91234-5678</telefone>
            <email>joao@email.com</email>
            <tipo_pessoa>0</tipo_pessoa>
            <ativo>1</ativo>
        </cliente>
    </clientes>
    <recursos>
        <recurso>
            <id>1</id>
            <nome>Microfone Shure SM58</nome>
            <descricao>Microfone dinâmico cardióide</descricao>
            <quantidade>10</quantidade>
            <preco_locacao>50.00</preco_locacao>
            <ativo>1</ativo>
        </recurso>
    </recursos>
</sge>
```

### Caracteres Especiais

O sistema escapa automaticamente:
- `&` vira `&amp;`
- `<` vira `&lt;`
- `>` vira `&gt;`
- `"` vira `&quot;`
- `'` vira `&apos;`

Exemplo:
```xml
<descricao>Aluguel de som &amp; luz para "eventos especiais"</descricao>
```

### Backup e Recuperação

**Fazer backup:**
1. Menu Ferramentas > Trade > Exportar
2. Selecione "Selecionar Todas"
3. Salve em local seguro como `backup_completo_2024.xml`

**Recuperar de backup:**
1. Menu Ferramentas > Trade > Importar
2. Marque "Sobrescrever"
3. Selecione "Selecionar Todas"
4. Escolha o arquivo de backup
5. Importe

**Dica:** Faça backups periódicos (semanal/mensal)

---

## Dicas

### Preços de Locação

Sempre faça compras pelo sistema para que o preço de locação seja atualizado automaticamente.

### Margem de Lucro

Ajuste a margem da produtora conforme necessário:
- 10% = preço competitivo
- 20% = preço padrão
- 30% = preço premium

### Backup dos Dados

Os arquivos .txt ficam na raiz do projeto. Copie-os regularmente:
```
clientes.txt
recursos.txt
eventos.txt
caixa.txt
etc.
```

### Organização

- Cadastre tudo antes de criar eventos
- Use descrições claras
- Atualize o estoque após eventos
- Lance no caixa assim que receber/pagar

---


## Resolução de Problemas

### "Cliente não encontrado"
Cadastre o cliente primeiro em Cadastros > Clientes

### "Recurso sem estoque"
Verifique a quantidade disponível em Cadastros > Recursos

### "Erro ao salvar"
Verifique se preencheu todos os campos obrigatórios

### "Preço de locação zerado"
Configure a margem de lucro em Cadastros > Produtora
