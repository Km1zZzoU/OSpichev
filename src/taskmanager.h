#pragma once

#include "externs.h"
#include "kernel.h"
#include "make.h"
#include "printer.h"

int counts = 10;
static cntxt* init_context(void (*func)());

void switch_task(cntxt** context) {
  // color_printf(yellow0, "eip: %h->", current_task->cntxt->eip);
  // color_printf(yellow0, "%h\n", current_task->next->cntxt->eip);

  if (dbg)
    color_printf(yellow0, "switch:\n");
  gdb_forks();
  // color_printf(yellow0, "%h\n", (*context)->eip);
  //
  // color_printf(yellow0, "esp: %h->%h\n", current_task->cntxt->esp, current_task->next->cntxt->esp);

  // color_printf(green1, "Main:%h->%h->%h->%h->%h->%h \n", Main.cntxt->eip, Main.next->cntxt->eip,
  //   Main.next->next->cntxt->eip, Main.next->next->next->cntxt->eip, Main.next->next->next->next->cntxt->eip,
  //   Main.next->next->next->next->next->cntxt->eip);

  if (flag_context_switch_happened == 1) {
    current_task->cntxt = *context;
    current_task = current_task->next;
    // printf("1\n");
  } else {
    // color_printf(red0, "set flag!!!\n");
    flag_context_switch_happened = 1;
  }
  // printf("2\n");

  // system_tick = 0;
  // color_printf(aqua0, "%h\n", Main.cntxt);
  // color_printf(yellow0, "esp: (%h) %h->%h\n", current_task->cntxt->esp, cntxt->esp, current_task->next->cntxt->esp);
  // color_printf(yellow0, "switch:\n");
  // switch_task(&cntxt);
  // *current_task->cntxt = *cntxt;
  // color_printf(bright_aqua, "cntx: %h\n", *context);
  // printf("3\n");
  *context = current_task->cntxt;
  if (dbg)
    printf("4");
  gdb_forks();

  // color_printf(yellow0, "%h\n", (*context)->eip);
  // current_task->cntxt = *context;
  // // color_printf(green1, "Main:%h->%h->%h->%h->%h->%h \n", Main.cntxt->eip, Main.next->cntxt->eip,
  // //   Main.next->next->cntxt->eip, Main.next->next->next->cntxt->eip, Main.next->next->next->next->cntxt->eip,
  // //   Main.next->next->next->next->next->cntxt->eip);
  // current_task = current_task->next;
  // // color_printf(green1, "Main:%h->%h->%h->%h->%h->%h \n", Main.cntxt->eip, Main.next->cntxt->eip,
  // //   Main.next->next->cntxt->eip, Main.next->next->next->cntxt->eip, Main.next->next->next->next->cntxt->eip,
  // //   Main.next->next->next->next->next->cntxt->eip);
  // **context = *current_task->cntxt;
  // // color_printf(green1, "Main:%h->%h->%h->%h->%h->%h \n", Main.cntxt->eip, Main.next->cntxt->eip,
  // //   Main.next->next->cntxt->eip, Main.next->next->next->cntxt->eip, Main.next->next->next->next->cntxt->eip,
  // //   Main.next->next->next->next->next->cntxt->eip);
  // // color_printf(aqua0, "hew context: %h\n", (*context)->eip);
  // // if (!--counts) {
  // //   __cli();
  // //   __loop();
  // // }
}

Task* init_task() {
  Task* task = make(sizeof(Task));
  task->next = task;
  task->cntxt = NULL;
  return task;
}

void append_task(Task* task, void (*func)()) {
  if (task->cntxt == NULL) {
    // printf("task == task->next\n");
    // color_printf(bright_aqua, "cntxt: %h\n", task->cntxt);
    task->cntxt = init_context(func);
    if (dbg)
      color_printf(bright_aqua, "task 1: %h %h\n", task, sizeof(Task));
  } else {
    // printf("task != task->next\n");
    Task* new_task = make(sizeof(Task));
    if (dbg)
      color_printf(bright_aqua, "task !1: %h\n", new_task);

    new_task->cntxt = init_context(func);
    new_task->next  = task->next;
    task->next = new_task;
  }
}

cntxt* init_context(void (*func)()) {
  cntxt* context = make(sizeof(cntxt));
  context->eax = 0;
  context->ebx = 0;
  context->ecx = 0;
  context->edx = 0;
  context->esi = 0;
  context->edi = 0;
  context->ebp = 0;
  context->esp = (u32)make(_4KB);
  context->esp += _4KB;
  context->eip = (u32)func;
  context->gs = 0x10;
  context->fs = 0x10;
  context->es = 0x10;
  context->ds = 0x10;
  context->cs = 0x8;
  context->ss = 0x0;
  context->e_code = 0xbed;
  context->e_flags = 0x0;
  context->vector = 0;
  context->p1 = 0;
  context->p2 = 0;
  context->p3 = 0;
  context->p4 = 0;
  context->p5 = 0;
  context->start_esp = 0;
  if (dbg)
    color_printf(red1, "init cntxt: %h %h esp: %h\n", context, sizeof(cntxt), context->esp);
  return context;
}

void go(Task* task) {
  current_task = task;
  // color_printf(red1, "ct:%h", current_task);
  // color_printf(red1, " %h\n", current_task->cntxt->eip);
  flag_context_switch_happened = 0;
  __sti();
}