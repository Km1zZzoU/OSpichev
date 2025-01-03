#pragma once
#include "typedef.h"

void* START = 0x00800000;
#define END 0xc0000000

void* make(u32 size) {
  // __cli();
  if ((u32)START + size > END)
    kpanic(-1, "out of memory");
  void* cur = START;
  START += size;
  return cur;
}