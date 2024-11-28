#pragma once
#include "windowmanager.h"

void syscall_print(Window* window, char* str) {
  if (!str)
    kpanic("error ptr for syscall_print %h", -1);
  c_out(window, str);
}