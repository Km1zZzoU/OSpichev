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
    __puts(ws[7], "HELLO, WORLD!!! ");
  }
}

void asd() {
  for (;;) {
    __puts(ws[1], "MY NAME IS OSPICHEV!!! ");
  }
}

void zxc() {
  for (;;) {
    __puts(ws[2], "SYSPRO OSE 2024 ");
  }
}

void vbn() {
  for (;;) {
    __puts(ws[3], "vbn8 ");
  }
}

void fgh() {
  for (;;) {
    __puts(ws[4], "f2gh ");
  }
}

void rty() {
  for (;;) {
    __puts(ws[5], "rt4y ");
  }
}

void rlsnd() {
  for (;;) {
    __puts(ws[6], "ds2jkn ");
  }
}

void mklk() {
  for (;;) {
    __puts(ws[8], "mkmk ");
  }
}

void kmain() {
  setup_paging();
  ws[0] = init_window_manager(2);
  ws[1] = append_window();
  ws[2] = append_window();
  ws[3] = append_window();
  ws[4] = append_window();
  ws[5] = append_window();
  ws[6] = append_window();
  ws[7] = append_window();
  ws[8] = append_window();
  // __loop();
  // init_printer();
  // color_printf(yellow0, "\nprinter init...\n\n");

  load_idt();
  init_pic();
  // color_printf(red0, "set IF...\n");
  Task* task = init_task();
  // color_printf(bright_red, "%h %h\n", hello, name);

  append_task(task, asd);
  append_task(task, zxc);
  append_task(task, vbn);
  append_task(task, fgh);
  append_task(task, rty);
  append_task(task, rlsnd);
  append_task(task, qwe);
  append_task(task, mklk);
  go(task);
  __loop();
}
