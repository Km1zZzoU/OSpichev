#pragma once

#include "externs.h"
#include "kernel.h"
#include "make.h"
#include "context.h"

Task* current_task;
int flag_context_switch_happened;

void switch_task(cntxt** context) {
  if (flag_context_switch_happened == 1) {
    current_task->cntxt = *context;
    current_task = current_task->next;
  } else {
    flag_context_switch_happened = 1;
  }

  *context = current_task->cntxt;
}

Task* init_task() {
  Task* task = make(sizeof(Task));
  task->next = task;
  task->cntxt = NULL;
  return task;
}

void append_task(Task* task, void (*func)()) {
  if (task->cntxt == NULL) {
    task->cntxt = init_context(func);
  } else {
    Task* new_task = make(sizeof(Task));

    new_task->cntxt = init_context(func);
    new_task->next  = task->next;
    task->next = new_task;
  }
}

void go(Task* task) {
  current_task = task;
  flag_context_switch_happened = 0;
  __sti();
}