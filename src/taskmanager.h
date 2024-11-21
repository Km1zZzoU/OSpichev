#pragma once

#include "kernel.h"
#include "make.h"
#include "externs.h"
#include "printer.h"

void init_task_manager(cntxt cntxt) {
  Main = (Task*)make(sizeof(Task));
  Main->cntxt = cntxt;
  Main->next = NULL;
  // color_printf(yellow0, "Main sp is %h\n", Main->cntxt.esp);

  current_task = Main;
  // vga_putn(red0, current_task->cntxt.eip, 16);
}

Task* get_last_task() {
  Task* last_task = current_task;
  // color_printf(red0, "\n%h->%h\n", last_task, last_task->next);
  for (;last_task->next != Main && last_task->next != NULL; last_task = last_task->next);
  // color_printf(red0, "\n%h\n", last_task);
  return last_task;
}

void append_task(cntxt task_context) {
  // color_printf(green1, "append task: %h, before: \n%h->%h-\n>%h->%h\n", task_context.esp, Main, Main->next,
  //   Main->next->next, Main->next->next->next);
  Task* new_task = make(sizeof(Task));
  new_task->cntxt = task_context;
  new_task->next = Main;
  Task* last_task = get_last_task();
  last_task->next = new_task;
  // color_printf(green1, "append task after: \n%h->%h-\n>%h->%h\n", Main, Main->next,
  //   Main->next->next, Main->next->next->next);
}

void go(void (*func)()) {
  __cli();
  // color_printf(green1, "start go %h\n", *func);
  cntxt context;
  context.eax = 0;
  context.ebx = 0;
  context.ecx = 0;
  context.edx = 0;
  context.esi = 0;
  context.edi = 0;
  context.ebp = 0;
  context.esp = (u32)make(_4KB);
  context.eip = (u32)func;
  context.gs = 0x10;
  context.fs = 0x10;
  context.es = 0x10;
  context.ds = 0x10;
  context.cs = 0x8;
  context.e_code = 0;
  context.e_flags = 0;
  context.vector = 228;
  append_task(context);
  __sti();
}