#ifndef CONFIG_H
#define CONFIG_H

typedef enum {
    PERS_MEMORIA,
    PERS_TEXTO,
    PERS_BINARIO
} TipoPersistencia;

/* Altere aqui para escolher a persistência global do sistema:
   PERS_MEMORIA  -> usa somente memória (temporário)
   PERS_TEXTO    -> salva em ".txt"
   PERS_BINARIO  -> salva em ".bin"
*/
#define TIPO_PERSISTENCIA PERS_TEXTO

#endif // CONFIG_H
