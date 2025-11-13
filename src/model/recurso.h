 #ifndef RECURSO_H
 #define RECURSO_H
 
 #define RECURSO_DESC_LEN 120
 #define RECURSO_CATEGORIA_LEN 60
 
 typedef struct {
	 int id;                    /* Código (único) */
	 char descricao[RECURSO_DESC_LEN];
	 char categoria[RECURSO_CATEGORIA_LEN];
	 int quantidade;
	 double preco_custo;        /* valor de aquisição */
	 double valor_locacao;      /* diária */
 } Recurso;
 
 #endif
