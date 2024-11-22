#pragma once

#include "kernel.h"
#include "make.h"
#include "externs.h"
#include "printer.h"

int counts = 1;

void switch_task(cntxt** context) {
  color_printf(yellow0, "switch:\n");
  color_printf(yellow0, "eip: %h->%h\n", current_task->cntxt->eip, current_task->next->cntxt->eip);
  color_printf(yellow0, "esp: %h->%h\n", current_task->cntxt->esp, current_task->next->cntxt->esp);

  color_printf(green1, "Main:%h->%h->%h->%h->%h->%h \n", Main.cntxt->eip, Main.next->cntxt->eip,
    Main.next->next->cntxt->eip, Main.next->next->next->cntxt->eip, Main.next->next->next->next->cntxt->eip,
    Main.next->next->next->next->next->cntxt->eip);

  current_task->cntxt = *context;
  color_printf(green1, "Main:%h->%h->%h->%h->%h->%h \n", Main.cntxt->eip, Main.next->cntxt->eip,
    Main.next->next->cntxt->eip, Main.next->next->next->cntxt->eip, Main.next->next->next->next->cntxt->eip,
    Main.next->next->next->next->next->cntxt->eip);
  current_task = current_task->next;
  color_printf(green1, "Main:%h->%h->%h->%h->%h->%h \n", Main.cntxt->eip, Main.next->cntxt->eip,
    Main.next->next->cntxt->eip, Main.next->next->next->cntxt->eip, Main.next->next->next->next->cntxt->eip,
    Main.next->next->next->next->next->cntxt->eip);
  *context = current_task->cntxt;
  color_printf(green1, "Main:%h->%h->%h->%h->%h->%h \n", Main.cntxt->eip, Main.next->cntxt->eip,
    Main.next->next->cntxt->eip, Main.next->next->next->cntxt->eip, Main.next->next->next->next->cntxt->eip,
    Main.next->next->next->next->next->cntxt->eip);
  color_printf(aqua0, "hew context: %h\n", (*context)->eip);
  if (!--counts) {
    __cli();
    __loop();
  }
}

void init_task_manager(cntxt* cntxt) {
  // Main = (Task*)make(sizeof(Task));
  // color_printf(aqua0, "%h\n", Main);

  Main.cntxt = cntxt;
  Main.next = &Main;
  // __cli();
  // __loop();
  // current_task = (Task*)make(sizeof(Task));
  current_task = &Main;
}

Task* get_last_task() {
  Task* last_task = current_task;
  if (last_task->next == &Main) {
    printf("main is last!!!!!!!!!!!!!!!!!!!!!!!\n");
  }
  for (;last_task->next != &Main && last_task->next != NULL; last_task = last_task->next);
  return last_task;
}

void append_task(cntxt* task_context) {
  Task* new_task = make(sizeof(Task));
  new_task->cntxt = task_context;
  new_task->next = &Main;
  Task* last_task = get_last_task();
  last_task->next = new_task;
}

void go(void (*func)()) {
  __cli();
  // color_printf(green1, "start go %h\n", *func);
  cntxt* context = make(sizeof(cntxt));
  context->eax = 0;
  context->ebx = 0;
  context->ecx = 0;
  context->edx = 0;
  context->esi = 0;
  context->edi = 0;
  context->ebp = 0;
  context->esp = (u32)make(_4KB);
  context->eip = (u32)func;
  context->gs = 0x10;
  context->fs = 0x10;
  context->es = 0x10;
  context->ds = 0x10;
  context->cs = 0x8;
  context->e_code = 0xbed;
  context->e_flags = 0x0;
  context->vector = 228;
  append_task(context);
  __sti();
}