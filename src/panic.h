#pragma once
#include "kernel.h"
#include "printer.h"
#include "externs.h"

void kpanic_handler(int vector) {
  kpanic("unhandled interupt without context: %d\n", vector);
}

void kpanic(char *msg, int vector) {
  init_printer();
  printf(msg, vector);
  __loop();
}