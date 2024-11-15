#include "kernel.h"
#include "externs.h"
#include "idt.h"
#include "make.h"
#include "panic.h"
#include "typedef.h"
#include "printer.h"
#include "pic.h"
#include "paging.h"

// ──────▄▀▄─────▄▀▄
// ─────▄█░░▀▀▀▀▀░░█▄
// ─▄▄──█░░░░░░░░░░░█──▄▄
// █▄▄█─█░░▀░░┬░░▀░░█─█▄▄█
//────────────────────────

int kmain() {
  init_printer();
  color_printf(yellow0, "printer init...\n\n");
  setup_paging();
  load_idt();
  init_pic();
  color_printf(red0, "set IF...\n\n");
  color_printf(red1, "setup complete! Welcome to the OSpichev!");
  __sti();
  // test_default_handler();
  __loop();
}
