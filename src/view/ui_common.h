#ifndef UI_COMMON_H
#define UI_COMMON_H

#include <iup.h>

/* Larguras padrão em pixels para padronização */
#define UI_LABEL_W   120
#define UI_W_ID       60
#define UI_W_SHORT   100
#define UI_W_MED     140
#define UI_W_LONG    220
#define UI_W_XLONG   300

/* Cria um par Label+Campo padronizado (label alinhado à direita) */
Ihandle* ui_pair(const char* label_text, Ihandle* field);

/* Cria uma linha com até dois pares lado a lado */
Ihandle* ui_row2(Ihandle* pair_left, Ihandle* pair_right);

/* Aplica margem e espaçamento padrão em um container vertical */
void ui_style_form(Ihandle* vbox);

/* Cria um HBox com botões alinhados à direita */
Ihandle* ui_buttons_right(Ihandle* b1, Ihandle* b2, Ihandle* b3);

/* Define largura em pixels para um campo (Text, etc) */
void ui_set_width_px(Ihandle* h, int px);

/* Cria um HBox com botões centralizados */
Ihandle* ui_buttons_center(Ihandle* b1, Ihandle* b2, Ihandle* b3);

/* Seleciona no list o item cujo ID armazenado em "ID_%d" é igual ao id */
void ui_find_and_set_item_by_id(Ihandle* list, int id);

#endif