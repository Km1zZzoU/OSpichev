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
  // __asm__ __volatile__ (
  //       "int $0"
  //   );
  // for (byte i = 0;; vga_putn(red0, i++, 16), foo());
  for (;;) {
    // __cli();
    printf("dbghi");
    gdb_forks();
    foo();
    // __sti();
  }
}

void name() {
  // for (byte i = 0;;vga_putn(purple0, i++, 16), bar());
  for (;;) {
    printf("dbgname");
    gdb_forks();
    // __cli();
    bar();
    // __sti();
  }
}

void biiz() {
  for (byte i = 0;;__cli(), vga_putn(orange0, i++, 16), biz(), __sti());
  biz();
}

void kmain() {
  dbg = 1;
  init_printer();
  color_printf(yellow0, "\nprinter init...\n\n");
  setup_paging();
  load_idt();
  init_pic();
  if (dbg)
    init_printer();
  color_printf(red0, "set IF...\n");
  // foo();
  Task* task = init_task();
  color_printf(bright_red, "%h %h\n", hello, name);
  append_task(task, hello);
  append_task(task, name);
  // color_printf(red1, "setup complete! Welcome to the OSpichev!\n\n\n\n\n\n");
  go(task);
  __loop();
  // foo();
  // biiz();
  __loop(); //1a565
}

