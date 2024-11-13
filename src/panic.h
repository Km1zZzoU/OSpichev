#pragma once
#include "kernel.h"
#include "printer.h"
#include "externs.h"

void panic_handler(int vector) {
  kpanic(vector, "%d");
}

void kpanic(char *msg, ...) {
  init_printer();
  printf(msg);
  __loop();
}