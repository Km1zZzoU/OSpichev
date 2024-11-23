#include "kernel.h"
#include "externs.h"
#include "idt.h"
#include "make.h"
#include "panic.h"
#include "typedef.h"
#include "printer.h"
#include "pic.h"
#include "paging.h"
#include "taskmanager.h"
#include "trahandler.h"

// ──────▄▀▄─────▄▀▄
// ─────▄█░░▀▀▀▀▀░░█▄
// ─▄▄──█░░░░░░░░░░░█──▄▄
// █▄▄█─█░░▀░░┬░░▀░░█─█▄▄█
//────────────────────────

int fib(int n) {
  if (n < 2) {
    return n;
  }
  return fib(n-1) + fib(n-2);
}

void hello() {
  for (;;) {
    __sti();
    printf("dbghi\n");
  }
}

void name() {
  for (;;) {
    bar();
  }
}

void biiz() {
  for (byte i = 0;;vga_putn(orange0, fib(i++), 16), biz());
}


void kmain() {
  init_printer();
  color_printf(yellow0, "\nprinter init...\n\n");
  setup_paging();
  load_idt();
  init_pic();
  color_printf(red0, "set IF...\n");
  Task* task = init_task();
  color_printf(bright_red, "%h %h\n", hello, name);
  append_task(task, hello);
  append_task(task, name);
  go(task);
  __loop();
}

