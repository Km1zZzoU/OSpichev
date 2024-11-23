#pragma once

void syscall_print(char* str) {
  if (!str)
    kpanic("error ptr for syscall_print %h", -1);
  printf(str);
  printf("\n");
}