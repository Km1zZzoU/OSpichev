#pragma once

#include "str.h"
#include "typedef.h"

symbol* fill_kostring(const char* cstring, const colorType color) {
  u32     len      = strlen(cstring);
  symbol* kostring = make(len * sizeof(symbol));
  for (u32 i = 0; i < len; i++) {
    kostring[i].character = cstring[i];
    kostring[i].color     = color;
  }
  return kostring;
}

u32 kostr_copy(symbol* buffer, const symbol* kostring) {
  u32 i;
  for (i = 0; kostring[i].character; i++)
    buffer[i] = kostring[i];
  return i;
}

u32 kolen(const symbol* s) {
  u32 len = 0;
  for (; s[len].character; len++)
    ;
  return len;
}
