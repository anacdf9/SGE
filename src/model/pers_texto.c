#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pers_texto.h"

static const char *ARQ_CLIENTES = "clientes.txt";

/* helper: escape simples para CSV (substitui ';' por ',') */
static void csv_escape(const char *in, char *out, int outsize) {
    int k = 0;
    for (int i = 0; in[i] != '\0' && k + 1 < outsize; i++) {
        char ch = in[i];
        if (ch == ';') {
            if (k + 1 < outsize) out[k++] = ',';
        } else {
            out[k++] = ch;
        }
    }
    out[k] = '\0';
}

void pers_texto_inicializar(void) {
    /* nada especial (arquivo criado quando salvarmos) */
}

void pers_texto_finalizar(void) {
    /* nada especial */
}

int pers_texto_salvar_cliente(Cliente c) {
    /* estratégia simples: ler todos, substituir por id se existir, senão append */
    Cliente *arr = NULL;
    int n = pers_texto_carregar_clientes(NULL, 0); /* obter número via leitura? we'll implement differently below */
    /* Implementação prática: reescrever todo o arquivo (safe): carregar todos para memória dinâmica */
    FILE *f = fopen(ARQ_CLIENTES, "r");
    Cliente temp;
    Cliente *lista = NULL;
    int total = 0;
    if (f) {
        while (fscanf(f, "%d;%99[^;];%149[^;];%29[^;];%29[^;];%79[^;];%79[^\n]\n",
                      &temp.id, temp.nome, temp.endereco, temp.cpf_cnpj, temp.telefone, temp.email, temp.contato) == 7) {
            lista = realloc(lista, sizeof(Cliente) * (total + 1));
            if (!lista) { fclose(f); return 0; }
            lista[total++] = temp;
        }
        fclose(f);
    }
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id == c.id) {
            lista[i] = c;
            found = 1;
            break;
        }
    }
    if (!found) {
        lista = realloc(lista, sizeof(Cliente) * (total + 1));
        if (!lista) return 0;
        lista[total++] = c;
    }
    /* reescreve */
    f = fopen(ARQ_CLIENTES, "w");
    if (!f) { free(lista); return 0; }
    for (int i = 0; i < total; i++) {
        /* escreve CSV separado por ; com limites */
        fprintf(f, "%d;%s;%s;%s;%s;%s;%s\n",
                lista[i].id,
                lista[i].nome,
                lista[i].endereco,
                lista[i].cpf_cnpj,
                lista[i].telefone,
                lista[i].email,
                lista[i].contato);
    }
    fclose(f);
    free(lista);
    return 1;
}

int pers_texto_carregar_clientes(Cliente *lista, int max) {
    FILE *f = fopen(ARQ_CLIENTES, "r");
    if (!f) return 0;
    int total = 0;
    Cliente temp;
    while (fscanf(f, "%d;%99[^;];%149[^;];%29[^;];%29[^;];%79[^;];%79[^\n]\n",
                  &temp.id, temp.nome, temp.endereco, temp.cpf_cnpj, temp.telefone, temp.email, temp.contato) == 7) {
        if (lista && total < max) lista[total] = temp;
        total++;
    }
    fclose(f);
    /* se lista==NULL, retornamos o total (mas a chamada feita aqui sempre passa buffer) */
    if (lista) return (total < max) ? total : max;
    return total;
}

int pers_texto_remover_cliente(int id) {
    FILE *f = fopen(ARQ_CLIENTES, "r");
    if (!f) return 0;
    Cliente *lista = NULL;
    int total = 0;
    Cliente temp;
    while (fscanf(f, "%d;%99[^;];%149[^;];%29[^;];%29[^;];%79[^;];%79[^\n]\n",
                  &temp.id, temp.nome, temp.endereco, temp.cpf_cnpj, temp.telefone, temp.email, temp.contato) == 7) {
        lista = realloc(lista, sizeof(Cliente) * (total + 1));
        if (!lista) { fclose(f); return 0; }
        lista[total++] = temp;
    }
    fclose(f);
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id == id) { /* remove: shift left */
            for (int j = i; j < total - 1; j++) lista[j] = lista[j + 1];
            total--;
            found = 1;
            break;
        }
    }
    if (!found) { free(lista); return 0; }
    f = fopen(ARQ_CLIENTES, "w");
    if (!f) { free(lista); return 0; }
    for (int i = 0; i < total; i++) {
        fprintf(f, "%d;%s;%s;%s;%s;%s;%s\n",
                lista[i].id,
                lista[i].nome,
                lista[i].endereco,
                lista[i].cpf_cnpj,
                lista[i].telefone,
                lista[i].email,
                lista[i].contato);
    }
    fclose(f);
    free(lista);
    return 1;
}
