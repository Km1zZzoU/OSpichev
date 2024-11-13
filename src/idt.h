#pragma once
#include "externs.h"
#include "kernel.h"
#include "typedef.h"
#include "keybord.h"

typedef struct {
  u32 edi;
  u32 esi;
  u32 ebp;
  u32 esp;
  u32 ebx;
  u32 edx;
  u32 ecx;
  u32 eax;
  u16 gs;
  u16 p1;
  u16 fs;
  u16 p2;
  u16 es;
  u16 p3;
  u16 ds;
  u16 p4;
  u32 vector;
  u32 e_code;
  u32 eip;
  u16 cs;
  u16 p5;
  u32 e_flags;
  u32 start_esp;
  u16 ss;
} cntxt;

void timer_trap () {
  // printf("%h ", tick++);
  return;
}

void kb_trap () {
  click_handler();
}

void __trap_handler(const cntxt *cntxt) {
  switch (cntxt->vector) {
  case 0x20:
    timer_trap();
    break;
  case 0x21:
    kb_trap();
    break;
  default:
    init_printer();
    printf("\nKernel panic: unhadled interrupt %h. Context:\n"
      "ESP = %h\n"
      "EAX = %h\nECX = %h\nEDX = %h\n"
      "EBX = %h\nEBP = %h\nESI = %h\n"
      "EDI = %h\n"
      "DS = %h ES = %h FS = %h GS = %h\n"
      "CS = %h SS = %h EIP = %h\n"
      "EFLAGS = %h error code = %h\n",
      cntxt->vector,
      cntxt->esp,
      cntxt->eax, cntxt->ecx, cntxt->edx,
      cntxt->ebx, cntxt->ebp, cntxt->esi,
      cntxt->edi,
      cntxt->ds, cntxt->es, cntxt->fs, cntxt->gs,
      cntxt->cs, cntxt->ss, cntxt->eip,
      cntxt->e_flags, cntxt->e_code);
    __loop();
    break;
  }
}

void load_idt() {
  gdst* idt = init_idtable();

  didt didt = {
    .size = (u16) (sizeof(gdst) * IDT_SIZE - 1),
    .ptr = (u32) idt,
    };

  __load_idt(&didt);
}

FUNC_GEN_ALL()
__FUNC_GEN_ALL()

gdst* init_idtable() {
  void* traps[256];

  FUNC_ASSIGN_ALL
  __FUNC_ASSIGN_ALL()

  gdst* idt = make(IDT_SIZE * sizeof(gdst));

  for (int vector = 0; vector < IDT_SIZE; vector++) {
    byte* handler = traps[vector];
    u16 low = (u16) handler;
    u16 seg_sel = 8;
    u16 flags = vector <= 0x30 ? 0b1000111100000000 : 0b1000111000000000;
    u16 high = (u32) handler >> 16;

    idt[vector].segment_selector = seg_sel;
    idt[vector].low_bits = low;
    idt[vector].flags = flags;
    idt[vector].high_bits = high;
  }

  return idt;
}
