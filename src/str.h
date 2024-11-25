#pragma once

u32 strlen(const char* s) {
  u32 len = 0;
  for (; s[len]; len++)
    ;
  return len;
}
