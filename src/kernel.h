#pragma once
void kmain();
 void __start__() {
  kmain();
}
#include "typedef.h"

void vga_clear();
void vga_puts(colorType color, char* c);
void vga_putc(colorType color, char c);
void vga_putn(colorType color, int x, int base);
void vga_printc(colorType color, char c, int* x, int* y);
void vga_prints(colorType color, char* str, int* x, int* y);
void printf(char* msg);
void shift_down();
void init_printer();
void color_printf(colorType color, char* fmt, ...);
void __debug_print_esp(u32 esp);
static void panic_handler(int vector);
void kpanic(char* msg, int vector);
void* make(u32 size);
void vga_draw(const u16 symbol[24], int x, int y, u32 color);
void draw_color(const u16 symbol[24], u32 color);
void load_idt();
void init_pic();
void timer_trap();
void kb_trap();
void click_handler();
byte system_tick = 0;
byte system_sec = 0;
byte system_min = 0;
byte system_hour = 0;

Task Main;
Task* current_task;
