#include "kernel.h"
#include "externs.h"
#include "idt.h"
#include "make.h"
#include "panic.h"
#include "typedef.h"
#include "printer.h"
#include "pic.h"

// ──────▄▀▄─────▄▀▄
// ─────▄█░░▀▀▀▀▀░░█▄
// ─▄▄──█░░░░░░░░░░░█──▄▄
// █▄▄█─█░░▀░░┬░░▀░░█─█▄▄█
//────────────────────────

int kmain() {
  init_printer();
  load_idt();
  init_pic();
  __sti();
  printf("start_loop:\nfor (int i = 1;; ++i) {\n  if (!i)\n    printf(\"int i is overflow\\n\");\n}\n");
  for (int i = 1;; ++i) {
    if (!i)
      printf("\nint i is overflow");
  }

}

