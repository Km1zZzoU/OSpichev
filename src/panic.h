#pragma once
#include "kernel.h"
#include "printer.h"
#include "externs.h"

__inline__
void panic_handler(int vector) {
  kpanic(vector, "%d");
}

void kpanic(int vector, char *msg, ...) {
  init_printer();
  printf(msg, vector);
  __loop();
}