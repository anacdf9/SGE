#include "validation.h"
#include <ctype.h>

void ui_digitos_apenas(const char* s, char* out, int max) {
  int j = 0;
  if (!s || !out || max <= 0) return;
  for (int i = 0; s[i] != '\0' && j < max - 1; ++i) {
    if (isdigit((unsigned char)s[i])) out[j++] = s[i];
  }
  out[j] = '\0';
}

int ui_valid_cpf_cnpj(const char* s) {
  char digits[32];
  ui_digitos_apenas(s, digits, sizeof(digits));
  int len = 0; while (digits[len] != '\0') len++;
  return (len == 11 || len == 14);
}

int ui_valid_telefone(const char* s) {
  char digits[32];
  ui_digitos_apenas(s, digits, sizeof(digits));
  int len = 0; while (digits[len] != '\0') len++;
  return (len == 10 || len == 11);
}
