#pragma once
#include "typedef.h"
int kmain();

void __start__() {
  kmain();
}


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
int curx = 0, cury = 0;
u16 font[256][24];
int tick = 0;

#pragma pack(push, 1)
typedef struct {
  u16 low_bits;
  u16 segment_selector;
  u16 flags;
  u16 high_bits;
} gdst;

gdst* init_idtable();

typedef struct {
  u16 size;
  u32 ptr;
} didt;
#pragma pack(push)