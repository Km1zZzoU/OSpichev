#pragma once

#include "kernel.h"

void update_time() {

  if (system_tick++ == 91) {
    system_tick = 1;
    system_sec += 5;
    if (system_sec == 60) {
      system_sec = 0;
      system_min++;
    }
    if (system_min == 60) {
      system_min = 0;
      system_hour++;
    }
  }
  int oldx = curx;
  int oldy = cury;

  curx = 0;
  cury = 0;
  for (int i = curx; i < 99; i++)
    vga_draw(font[0xdb], i, 0, bg0);

  for (u32 i = 0; ws[i]; i++)
    color_printf(orange0, " w%h:%h ", i, ws[i]->size_buff);
  color_printf(orange0, "%h ", count_sw);
  vga_putn(purple0, START, 16);
  vga_puts(red1, " time:");
  vga_putn(red0, system_hour, 10);
  vga_putc(red1, '|');
  vga_putn(red0, system_min, 10);
  vga_putc(red1, '|');
  vga_putn(red0, system_sec, 10);
  vga_putc(red1, '|');
  vga_putn(yellow1, system_tick, 10);
  curx = oldx;
  cury = oldy;
}
