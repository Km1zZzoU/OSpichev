#pragma once
#include "externs.h"
#include "kernel.h"
#include "keybord.h"
#include "panic.h"
#include "syscalls.h"
#include "typedef.h"
#include "taskmanager.h"

#define IDT_SIZE 256

#define __FUNC_GEN(x) extern void __trap_##x();
#define __FUNC_GEN_2(b1) __FUNC_GEN(b1##0) __FUNC_GEN(b1##1)
#define __FUNC_GEN_4(b2) __FUNC_GEN_2(b2##0) __FUNC_GEN_2(b2##1)
#define __FUNC_GEN_8(b3) __FUNC_GEN_4(b3##0) __FUNC_GEN_4(b3##1)
#define __FUNC_GEN_16(b4) __FUNC_GEN_8(b4##0) __FUNC_GEN_8(b4##1)
#define __FUNC_GEN_32(b5) __FUNC_GEN_16(b5##0) __FUNC_GEN_16(b5##1)
#define __FUNC_GEN_ALL() __FUNC_GEN_32(0) __FUNC_GEN(100000) __FUNC_GEN(100001) __FUNC_GEN(1000010)

#define __FUNC_ASSIGN(x) traps[0b##x] = __trap_##x;
#define __FUNC_ASSIGN_2(b1) __FUNC_ASSIGN(b1##0) __FUNC_ASSIGN(b1##1)
#define __FUNC_ASSIGN_4(b2) __FUNC_ASSIGN_2(b2##0) __FUNC_ASSIGN_2(b2##1)
#define __FUNC_ASSIGN_8(b3) __FUNC_ASSIGN_4(b3##0) __FUNC_ASSIGN_4(b3##1)
#define __FUNC_ASSIGN_16(b4) __FUNC_ASSIGN_8(b4##0) __FUNC_ASSIGN_8(b4##1)
#define __FUNC_ASSIGN_32(b5) __FUNC_ASSIGN_16(b5##0) __FUNC_ASSIGN_16(b5##1)
#define __FUNC_ASSIGN_ALL() __FUNC_ASSIGN_32(0) __FUNC_ASSIGN(100000) __FUNC_ASSIGN(100001) __FUNC_ASSIGN(1000010)

#define FUNC_GEN(x) static void trap_##x() { kpanic_handler(0b##x); } __FUNC_GEN(0b##x)
#define FUNC_GEN_2(b1) FUNC_GEN(b1##0) FUNC_GEN(b1##1)
#define FUNC_GEN_4(b2) FUNC_GEN_2(b2##0) FUNC_GEN_2(b2##1)
#define FUNC_GEN_8(b3) FUNC_GEN_4(b3##0) FUNC_GEN_4(b3##1)
#define FUNC_GEN_16(b4) FUNC_GEN_8(b4##0) FUNC_GEN_8(b4##1)
#define FUNC_GEN_32(b5) FUNC_GEN_16(b5##0) FUNC_GEN_16(b5##1)
#define FUNC_GEN_64(b6) FUNC_GEN_32(b6##0) FUNC_GEN_32(b6##1)
#define FUNC_GEN_128(b7) FUNC_GEN_64(b7##0) FUNC_GEN_64(b7##1)
#define FUNC_GEN_ALL() FUNC_GEN_128(0) FUNC_GEN_128(1)

#define FUNC_ASSIGN(x) traps[0b##x] = trap_##x;
#define FUNC_ASSIGN_2(b1) FUNC_ASSIGN(b1##0) FUNC_ASSIGN(b1##1)
#define FUNC_ASSIGN_4(b2) FUNC_ASSIGN_2(b2##0) FUNC_ASSIGN_2(b2##1)
#define FUNC_ASSIGN_8(b3) FUNC_ASSIGN_4(b3##0) FUNC_ASSIGN_4(b3##1)
#define FUNC_ASSIGN_16(b4) FUNC_ASSIGN_8(b4##0) FUNC_ASSIGN_8(b4##1)
#define FUNC_ASSIGN_32(b5) FUNC_ASSIGN_16(b5##0) FUNC_ASSIGN_16(b5##1)
#define FUNC_ASSIGN_64(b6) FUNC_ASSIGN_32(b6##0) FUNC_ASSIGN_32(b6##1)
#define FUNC_ASSIGN_128(b7) FUNC_ASSIGN_64(b7##0) FUNC_ASSIGN_64(b7##1)
#define FUNC_ASSIGN_ALL FUNC_ASSIGN_128(0) FUNC_ASSIGN_128(1)

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

void timer_trap () {
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
  curx = 85;
  cury = 0;
  for (int i = curx; i < 99; i++)
    vga_draw(font[0xdb], i, 0, bg0);
  vga_puts(red1, "time:");
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

void kb_trap () {
  click_handler();
}

int count_switch = 3;

void __trap_handler(cntxt *cntxt) {
  switch (cntxt->vector) {
  case 0x20:
    if (!system_tick) {
      init_task_manager(*cntxt);
      // __loop();
    } else if (Main->next && system_tick & 0x1f) {
      count_switch--;
      // timer_trap();
      // switch_task();
      // __loop();

      color_printf(yellow0, "switch:\n");
      color_printf(yellow0, "eip: (%h) %h->%h\n", current_task->cntxt.eip, cntxt->eip, current_task->next->cntxt.eip);
      color_printf(yellow0, "esp: (%h) %h->%h\n", current_task->cntxt.esp, cntxt->esp, current_task->next->cntxt.esp);
      current_task->cntxt = *cntxt;
      current_task = current_task->next;
      *cntxt = current_task->cntxt;
      if (!count_switch)
        __loop();
    }
    timer_trap();
    break;
  case 0x21:
    kb_trap();
    break;
  case 0x42:
    syscall_print((char *)cntxt->eax);
    break;
  default:
    // init_printer();
    __cli();
    color_printf(red1, "\nKernel panic: unhadled interrupt %h. Context:\n"
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
  if (cntxt->vector >= 0x20 || cntxt->vector == 228) {
    __sti();
    if (cntxt->vector < 0x30 || cntxt->vector == 228) {
      __eoi();
    }
  }
}

void load_idt() {
  color_printf(orange0, "start load_idt...\n");
  gdst* idt = init_idtable();

  didt didt = {
    .size = (u16) (sizeof(gdst) * IDT_SIZE - 1),
    .ptr = (u32) idt,
    };
  color_printf(orange0, "  call lidt...\n");
  __load_idt(&didt);
  color_printf(orange0, "idt have been loaded...\n\n");
}

FUNC_GEN_ALL()
__FUNC_GEN_ALL()

gdst* init_idtable() {
  color_printf(orange0, "  start init table...\n");
  void* traps[256];

  FUNC_ASSIGN_ALL
  __FUNC_ASSIGN_ALL()


  gdst* idt = make(IDT_SIZE * sizeof(gdst));
  color_printf(orange0, "    alloc memory for idt...\n");

  for (int vector = 0; vector < IDT_SIZE; vector++) {
    byte* handler = traps[vector];
    u16 low = (u16) handler;
    u16 seg_sel = 8;
    u16 flags = vector < 0x20 ? 0b1000111100000000 : 0b1000111000000000;
    // flags = 0b1000111100000000;
    u16 high = (u32) handler >> 16;

    idt[vector].segment_selector = seg_sel;
    idt[vector].low_bits = low;
    idt[vector].flags = flags;
    idt[vector].high_bits = high;
  }
  color_printf(orange0, "    complete idt...\n");
  return idt;
}
