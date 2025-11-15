#ifndef VALIDATION_H
#define VALIDATION_H

#include <iup.h>

/* Remove caracteres não numéricos de s, escreve em out (limitado por max) */
void ui_digits_only(const char* s, char* out, int max);

/* CPF/CNPJ válido (apenas comprimento): 11 ou 14 dígitos */
int ui_valid_cpf_cnpj(const char* s);

/* Telefone válido: 10 ou 11 dígitos */
int ui_valid_phone(const char* s);

#endif