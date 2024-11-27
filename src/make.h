#pragma once
#include "typedef.h"

// void* START = 0xc0000000;
// #define END 0xf0000000
void* START = 0x00800000;
#define END 0xc0000000

void* make(u32 size) {
  if ((u32)START + size > END)
    kpanic(-1, "out of memory");
  void* cur = START;
  START += size;
  return cur;
}