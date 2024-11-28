#pragma once

#include "str.h"
#include "typedef.h"

symbol* fill_kostring(const char* cstring, const colorType color) {
  u32 len = strlen(cstring);

  symbol* kostring = make((len + 1) * sizeof(symbol));
  for (u32 i = 0; i < len; i++) {
    kostring[i].character = cstring[i];
    kostring[i].color     = color;
  }
  kostring[len].character = 0;
  return kostring;
}

u32 kostr_copy(symbol* buffer, const symbol* kostring) {
  u32 i;
  for (i = 0; kostring[i].character; i++)
    buffer[i] = kostring[i];
  buffer[i].character = 0;
  return i;
}

u32 kolen(const symbol* s) {
  return s->character ? strlen(++s) + 1 : 0;
}
