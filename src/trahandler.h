#pragma once
#include "kernel.h"
#include "printer.h"
#include "syscalls.h"
#include "taskmanager.h"

#define SPEED_SWITCH 0

void __handler(cntxt* cntxt) {
  int IF = cntxt->e_flags & (1 << 9);
  switch (cntxt->vector) {
    case 0x20:
      if (current_task && !(system_tick & ((1 << SPEED_SWITCH) - 1)))
        switch_task(&cntxt);
      update_time();
      if (!cntxt->vector) {
        __eoi();
      }
      break;
    case 0x21:
      kb_trap();
      break;
    case 0x42:
      syscall_print((Window*)cntxt->ebx, (char*)cntxt->eax);
      break;
    default:
      // init_printer();
      __cli();
      __loop();
      color_printf(red1,
                   "\nKernel panic: unhadled interrupt %h. Context:\n"
                   "ESP = %h\n"
                   "EAX = %h\nECX = %h\nEDX = %h\n"
                   "EBX = %h\nEBP = %h\nESI = %h\n"
                   "EDI = %h\n"
                   "DS = %h ES = %h FS = %h\n GS = %h\n"
                   "EIP = %h, error code = %h\n",
                   cntxt->vector, cntxt->esp, cntxt->eax, cntxt->ecx,
                   cntxt->edx, cntxt->ebx, cntxt->ebp, cntxt->esi, cntxt->edi,
                   cntxt->ds, cntxt->es, cntxt->fs, cntxt->gs, cntxt->eip,
                   cntxt->e_code);
      break;
  }
  cntxt->e_flags |= IF;
  if (cntxt->vector >= 0x20 && cntxt->vector <= 0x30) {
    __eoi();
  }
}