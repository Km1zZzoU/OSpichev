#pragma once
#include "make.h"
#include "typedef.h"
#define STACK_SIZE _4MB

cntxt* init_context(void (*func)()) {
  u32    stack       = (u32)make(STACK_SIZE);
  cntxt* context     = make(sizeof(cntxt));
  context->eax       = 0;
  context->ebx       = 0;
  context->ecx       = 0;
  context->edx       = 0;
  context->esi       = 0;
  context->edi       = 0;
  context->ebp       = 0;
  context->esp       = stack + STACK_SIZE;
  context->eip       = (u32)func;
  context->gs        = 0x10;
  context->fs        = 0x10;
  context->es        = 0x10;
  context->ds        = 0x10;
  context->cs        = 0x8;
  context->ss        = 0x0;
  context->e_code    = 0x0;
  context->e_flags   = 1 << 9;
  context->vector    = 0;
  context->p1        = 0;
  context->p2        = 0;
  context->p3        = 0;
  context->p4        = 0;
  context->p5        = 0;
  context->start_esp = 0;
  return context;
}