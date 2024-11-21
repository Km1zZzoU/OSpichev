#pragma once
#include "typedef.h"
#include "printer.h"
#include "externs.h"

#define PtrPTEs 0x100000
#define PtrPDEs (PtrPTEs + _4MB)

void setup_paging() {
  color_printf(purple1, "start setup paging...\n");
  u32 start_addr = PtrPTEs;
  *(u32*) start_addr = 0b011; //off first table
  start_addr += _4B;
  color_printf(purple1, "  off first page...\n");

  const u32 default_PTE = 0b11;
  for (u32 frame_address = _4KB; frame_address >= _4KB; frame_address += _4KB) {
    const u32 PTE = default_PTE | frame_address;
    *(u32*) start_addr = PTE;
    start_addr += _4B;
  }
  color_printf(purple1, "  PTEs have been configured on ");
  color_printf(red1, "0x%h-0x%h", PtrPTEs, PtrPDEs - 1);
  color_printf(purple1, "...\n");

  start_addr = PtrPDEs;
  const u32 default_PDE = default_PTE;
  for (u32 pte_address = PtrPTEs; pte_address < PtrPDEs; pte_address += _4KB) {
    const u32 PDE = default_PDE | pte_address;
    *(u32*) start_addr = PDE;
    start_addr += _4B;
  }
  color_printf(purple1, "  PDEs have been configured on ");
  color_printf(red1, "0x%h-0x%h", PtrPDEs, PtrPDEs + _4KB - 1);
  color_printf(purple1, "...\n  call __setup_paging...\n");
  __setup_paging(PtrPDEs);
  color_printf(purple1, "paging have been configured...\n\n");
}
