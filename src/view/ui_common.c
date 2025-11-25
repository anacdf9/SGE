#include "ui_common.h"
#include <stdio.h>
#include <stdlib.h>

static void set_label_std(Ihandle* lbl) {
  IupSetAttribute(lbl, "ALIGNMENT", "ARIGHT:ACENTER");
  char sz[32];
  sprintf(sz, "%dx", UI_LABEL_W);
  IupSetAttribute(lbl, "RASTERSIZE", sz);
}

void ui_set_width_px(Ihandle* h, int px) {
  char sz[32];
  sprintf(sz, "%dx", px);
  IupSetAttribute(h, "RASTERSIZE", sz);
}

Ihandle* ui_pair(const char* label_text, Ihandle* field) {
  Ihandle* lbl = IupLabel(label_text);
  set_label_std(lbl);
  Ihandle* h = IupHbox(lbl, field, NULL);
  IupSetAttribute(h, "GAP", "6");
  return h;
}

Ihandle* ui_row2(Ihandle* pair_left, Ihandle* pair_right) {
  if (!pair_right) {
    Ihandle* h = IupHbox(pair_left, NULL);
    IupSetAttribute(h, "GAP", "12");
    return h;
  }
  Ihandle* h = IupHbox(pair_left, pair_right, NULL);
  IupSetAttribute(h, "GAP", "24");
  return h;
}

void ui_style_form(Ihandle* vbox) {
  IupSetAttribute(vbox, "MARGIN", "10x10");
  IupSetAttribute(vbox, "GAP", "6");
}

Ihandle* ui_buttons_right(Ihandle* b1, Ihandle* b2, Ihandle* b3) {
  return IupHbox(IupFill(), b1, b2, b3, NULL);
}

Ihandle* ui_buttons_center(Ihandle* b1, Ihandle* b2, Ihandle* b3) {
  return IupHbox(IupFill(), b1, b2, b3, IupFill(), NULL);
}

void ui_find_and_set_item_by_id(Ihandle* list, int id) {
  if (!list || id <= 0) return;
  int count = IupGetInt(list, "COUNT");
  for (int i = 1; i <= count; i++) {
    const char* id_str = IupGetAttributeId(list, "ID_%d", i);
    if (id_str && atoi(id_str) == id) {
      char buf[16];
      sprintf(buf, "%d", i);
      IupSetAttribute(list, "VALUE", buf);
      return;
    }
  }
}
