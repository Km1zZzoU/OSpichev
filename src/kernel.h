#pragma once
#include "typedef.h"
int kmain();

void __start__() {
  kmain();
}

void vga_clear();
void vga_puts(char* c);
void vga_putc(char c);
void vga_putn(int x, int base);
void vga_printc(char c, int* x, int* y);
void vga_prints(char* str, int* x, int* y);
void shift_down();
void init_printer();
void printf(char* fmt, ...);
short get_colar(int n);
void kpanic(char *msg, ...);
void* make(u32 size);
void load_idt();
void init_pic();
void timer_trap();
void kb_trap();

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