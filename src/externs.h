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
extern void test_default_handler();