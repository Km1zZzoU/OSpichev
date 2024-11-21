#pragma once

void syscall_print(char* str) {
  printf(str);
  printf("\n");
  __sti();
}