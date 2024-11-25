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


void hello() {
  for (;;) {
    __sti();
    printf("dbghi\n");
  }
}

void qwe() {
  for (;;)
    __puts(ws[0], "qwe");
}

void name() {
  for (;;)
    __puts(ws[1], "My name is OSpichev!");
}

void kmain() {
  ws[0] = init_window_manager(1);
  ws[1] = append_window();

  // c_out(main, "Hello, world!\n");
  // c_out(main,
  // "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq123456");
  // c_out(secondary,
  //       "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq1234567");
  // c_out(secondary, "My name is Ospichev!\n");

  // __loop();
  // init_printer();
  // color_printf(yellow0, "\nprinter init...\n\n");

  setup_paging();
  load_idt();
  init_pic();
  // color_printf(red0, "set IF...\n");
  Task* task = init_task();
  // color_printf(bright_red, "%h %h\n", hello, name);
  // __puts(ws[0], "My name is OSpichev!");

  for (u32 i = 0; i < 116; i++)
    __puts(ws[0], "My name is OSpichev!");
  __loop();
  append_task(task, qwe);
  append_task(task, name);
  go(task);
  __loop();
}
