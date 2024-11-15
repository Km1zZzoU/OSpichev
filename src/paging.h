#pragma once
#include "typedef.h"
#include "printer.h"
#include "externs.h"

#define _4B  (1<< 2)
#define _4KB (1<<12)
#define _4MB (1<<22)
#define PtrPTEs 0x100000
#define PtrPDEs (PtrPTEs + _4MB)

void setup_paging() {
  // color_printf(purple1, "start setup paging...\n");
  u32 start_addr = PtrPTEs;
  *(u32*) start_addr = 0; //off first table
  // color_printf(purple1, "  off first page...\n");

  u32 default_PTE = 0b111000000111;
  for (u32 frame_address = _4KB; frame_address; frame_address += _4KB) {
    u32 PTE = default_PTE | frame_address;
    *(u32*) start_addr = PTE;
    start_addr += _4B;
  }

  // color_printf(purple1, "  PTEs have been configured...\n");
  start_addr = PtrPDEs;
  u32 default_PDE = default_PTE;
  for (u32 pte_address = PtrPTEs; pte_address < PtrPDEs; pte_address += _4KB) {
    u32 PDE = default_PDE | pte_address;
    *(u32*) start_addr = PDE;
    start_addr += _4B;
  }
  // color_printf(purple1, "  PDEs have been configured...\n");

  // color_printf(purple1, "  call __setup_paging...\n");
  __setup_paging(PtrPDEs);
  color_printf(purple1, "paging have been configured...\n");
}
