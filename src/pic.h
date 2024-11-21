#pragma once
#include "externs.h"

#define MASTER_COMMAND 0x20
#define MASTER_DATA 0x21
#define SLAVE_COMMAND 0xA0
#define SLAVE_DATA 0xA1

void init_pic() {
  color_printf(green0, "start init pic...\n");
  __const__ byte icw1 = 0b00010001;
  __const__ byte icw2_m = 0x20;
  __const__ byte icw2_s = icw2_m + 8;
  __const__ byte icw3_m = 0b100;
  __const__ byte icw3_s = 2;
  __const__ byte icw4 = 1;
  color_printf(green0, "  start init master...\n");
  __outb(MASTER_COMMAND, icw1);
  __outb(MASTER_DATA, icw2_m);
  __outb(MASTER_DATA, icw3_m);
  __outb(MASTER_DATA, icw4);
  color_printf(green0, "  master init...\n");
  color_printf(green0, "  start slave master...\n");
  __outb(SLAVE_COMMAND, icw1);
  __outb(SLAVE_DATA, icw2_s);
  __outb(SLAVE_DATA, icw3_s);
  __outb(SLAVE_DATA, icw4);
  color_printf(green0, "  slave init...\n");
  color_printf(green0, "  start init kb and timer...\n");
  // __outb(MASTER_DATA, ~0); //off all
  __outb(MASTER_DATA, ~1); //on timer
  __outb(MASTER_DATA, ~2); //on keybd
  __outb(MASTER_DATA, ~3); //on all
  color_printf(green0, "  kb and timer init...\n");
  color_printf(green0, "pic init...\n\n");
}
