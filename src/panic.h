#pragma once
#include "externs.h"
#include "kernel.h"
#include "printer.h"

void kpanic_handler(int vector) {
  kpanic("unhandled interupt without context: %d\n", vector);
}

void kpanic(char* msg, int vector) {
  init_printer();
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < h; y++) {
      u32 crd = x + y * w;
      if (x < 3 || x >= w - 3 || y < 3 || y >= h - 3) {
        getptr(0, 0, crd) = red1;
      } else {
        getptr(0, 0, crd) = red0;
      }
    }
  }
  curx = 40;
  cury = 20;
  color_printf(0, msg, vector);
  __loop();
}