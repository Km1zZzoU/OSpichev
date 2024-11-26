#pragma once

u32 strlen(const char* s) {
  return *s ? strlen(++s) + 1 : 0;
}
