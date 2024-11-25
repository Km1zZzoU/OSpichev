#pragma once
#include "typedef.h"

void* START = 0x800000;
#define END 0x10800000

void* make(u32 size) {
  if ((u32)START + size > (u32)END)
    kpanic(-1, "out of memory");
  void* cur = START;
  START += size;
  return cur;
}