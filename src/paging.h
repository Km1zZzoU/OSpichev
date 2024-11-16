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
  color_printf(purple1, "start setup paging...\n");
  u32 start_addr = PtrPTEs;
  *(u32*) start_addr = 0b110; //off first table
  start_addr += _4B;
  // color_printf(purple1, "  off first page...\n");

  u32 default_PTE = 0b110;
  for (u32 frame_address = _4KB; frame_address >= _4KB; frame_address += _4KB) {
    u32 PTE = default_PTE | frame_address;
    *(u32*) start_addr = PTE;
    start_addr += _4B;
  }

  color_printf(purple1, "PTEs:\n");
  for (u16 i = 0; i < 6; i++) {
    color_printf(green1, "%h: %h->%h;\n",i * 4 + PtrPTEs, i, *(u32*) (i * 4 + PtrPTEs));
  }
  //__loop();

  // color_printf(purple1, "  PTEs have been configured...\n");
  start_addr = PtrPDEs;
  u32 default_PDE = default_PTE;
  for (u32 pte_address = PtrPTEs; pte_address < PtrPDEs; pte_address += _4KB) {
    // color_printf(green1, "%h -> %h\n", i++, *(u32*) (pte_address + 0));
    // if (i == 40)
    //   __loop();
    u32 PDE = default_PDE | pte_address;
    *(u32*) start_addr = PDE;
    start_addr += _4B;
    //break;
  }
  // color_printf(purple1, "  PDEs have been configured...\n");
  color_printf(purple1, "PDEs: \n");
  for (u16 i = 0; i < 6; i++) {
    color_printf(green1, "%h: %h->%h->%h;\n", i * 4 + 0x500000, i, *(u32*) (i * 4 + 0x500000), *(u32*) ((*(u32*) (i * 4 + 0x500000)) & 0xFFFFF000));
  }
  // __loop();
  color_printf(purple1, "  call __setup_paging...\n");
  __setup_paging(PtrPDEs);
  __loop();
  color_printf(purple1, "paging have been configured...\n");
}
