#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pers_binario.h"

static const char *ARQ_CLIENTES_BIN = "clientes.bin";

/* formato: sequência de registros Cliente com tamanho fixo (struct) */
/* NOTE: use fwrite/fread com structs com campos char[] fixos (como o nosso Cliente) */

void pers_binario_inicializar(void) {
    /* nada especial */
}

void pers_binario_finalizar(void) {
    /* nada especial */
}

int pers_binario_salvar_cliente(Cliente c) {
    /* carregar todos, substituir se id existe, senão append e gravar novamente */
    FILE *f = fopen(ARQ_CLIENTES_BIN, "rb");
    Cliente *lista = NULL;
    int total = 0;
    if (f) {
        Cliente temp;
        while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
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
    f = fopen(ARQ_CLIENTES_BIN, "wb");
    if (!f) { free(lista); return 0; }
    fwrite(lista, sizeof(Cliente), total, f);
    fclose(f);
    free(lista);
    return 1;
}

int pers_binario_carregar_clientes(Cliente *lista, int max) {
    FILE *f = fopen(ARQ_CLIENTES_BIN, "rb");
    if (!f) return 0;
    int total = 0;
    Cliente temp;
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
        if (lista && total < max) lista[total] = temp;
        total++;
    }
    fclose(f);
    if (lista) return (total < max) ? total : max;
    return total;
}

int pers_binario_remover_cliente(int id) {
    FILE *f = fopen(ARQ_CLIENTES_BIN, "rb");
    if (!f) return 0;
    Cliente *lista = NULL;
    int total = 0;
    Cliente temp;
    while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
        lista = realloc(lista, sizeof(Cliente) * (total + 1));
        if (!lista) { fclose(f); return 0; }
        lista[total++] = temp;
    }
    fclose(f);
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (lista[i].id == id) {
            for (int j = i; j < total - 1; j++) lista[j] = lista[j + 1];
            total--;
            found = 1;
            break;
        }
    }
    if (!found) { free(lista); return 0; }
    f = fopen(ARQ_CLIENTES_BIN, "wb");
    if (!f) { free(lista); return 0; }
    fwrite(lista, sizeof(Cliente), total, f);
    fclose(f);
    free(lista);
    return 1;
}