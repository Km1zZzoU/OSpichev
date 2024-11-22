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
  for (byte i = 0;;__cli(), vga_putn(red0, i++, 16), foo(), __sti());
  foo();
}

void name() {
  for (byte i = 0;;__cli(), vga_putn(purple0, i++, 16), bar(), __sti());
  bar();
}

void biiz() {
  for (byte i = 0;;__cli(), vga_putn(orange0, i++, 16), biz(), __sti());
  biz();
}

void start_multitasking() {
  __sti();
  for (;!system_tick;);
}

void kmain() {
  init_printer();
  color_printf(yellow0, "\nprinter init...\n\n");
  setup_paging();
  load_idt();
  init_pic();
  color_printf(red0, "set IF...\n");
  start_multitasking();
  // color_printf(red1, "setup complete! Welcome to the OSpichev!\n\n\n\n\n\n");
  go(hello);
  printf("go hello\n");
  go(name);
  printf("go name\n");
  biiz();
  // go(biiz, "say biiz");
  // printf("go bizz");
  __loop(); //1a565
}

























