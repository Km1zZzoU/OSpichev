#include "kernel.h"
#include "externs.h"
#include "idt.h"
#include "make.h"
#include "paging.h"
#include "panic.h"
#include "pic.h"
#include "printer.h"
#include "symbol.h"
#include "taskmanager.h"
#include "trahandler.h"
#include "typedef.h"
#include "windowmanager.h"

// ──────▄▀▄─────▄▀▄
// ─────▄█░░▀▀▀▀▀░░█▄
// ─▄▄──█░░░░░░░░░░░█──▄▄
// █▄▄█─█░░▀░░┬░░▀░░█─█▄▄█
// ────────────────────────

int fib(int n) {
  if (n < 2) {
    return n;
  }
  return fib(n - 1) + fib(n - 2);
}


void qwe() {
  for (;;) {
    __puts(ws[0], "0");
  }
}

void asd() {
  for (;;) {
    __puts(ws[1], "1");
  }
}

void zxc() {
  for (;;) {
    __puts(ws[2], "2");
  }
}

void kmain() {
  ws[0] = init_window_manager(2);
  ws[1] = append_window();
  // ws[2] = append_window();

  // init_printer();
  // color_printf(yellow0, "\nprinter init...\n\n");

  setup_paging();
  load_idt();
  init_pic();
  // color_printf(red0, "set IF...\n");
  Task* task = init_task();
  // color_printf(bright_red, "%h %h\n", hello, name);

  append_task(task, qwe);
  append_task(task, asd);
  // append_task(task, zxc);
  go(task);
  __loop();
}
