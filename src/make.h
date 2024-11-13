#pragma once
#include "typedef.h"

static void* START = 0x100000;
#define END 0x400000

void* make(u32 size) {
  if ((u32) START + size > (u32) END) {
    kpanic(-1, "out of memory");
  }
  void* cur = START;
  START += size;
  return cur;
}