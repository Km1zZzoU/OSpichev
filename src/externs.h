#pragma once
#include "typedef.h"
extern void __loop();
extern void __sti();
extern void __cli();
extern void __load_idt(void* idt_ptr);
extern void __outb(u16 port, byte data);
extern byte __inb(u16 port);
extern void __timer_trap();
extern void __kb_trap();
extern void __setup_paging(int pd);
extern void __eoi();
extern void test_default_handler();
extern void foo();
extern void bar();
extern void biz();
extern void print_stack();
