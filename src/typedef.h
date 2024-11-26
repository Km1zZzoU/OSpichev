#pragma once

typedef unsigned char      byte;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned int       colorType;
typedef unsigned long long u64;

#pragma pack(push, 1)
typedef struct {
  u32 edi;
  u32 esi;
  u32 ebp;
  u32 esp;
  u32 ebx;
  u32 edx;
  u32 ecx;
  u32 eax;
  u16 gs;
  u16 p1;
  u16 fs;
  u16 p2;
  u16 es;
  u16 p3;
  u16 ds;
  u16 p4;
  u32 vector;
  u32 e_code;
  u32 eip;
  u16 cs;
  u16 p5;
  u32 e_flags;
  u32 start_esp;
  u16 ss;
} cntxt;
#pragma pack(pop)

typedef struct Task {
  cntxt*       cntxt;
  struct Task* next;
} Task;

typedef struct symbol {
  colorType color;
  byte      character;
} symbol;

typedef struct Window {
  u32            x0;
  u32            y0;
  byte           width;  // in symbols 16x24
  byte           height; // in symbols 16x24
  symbol*        symbols;
  u32            size_buff;
  u32            index_for_show;
  struct Window* next;
} Window;

typedef struct {
  byte    id;
  Window* window;
  u32     window_count;
  u32     current_window;
} WorkSpace;

typedef struct {
  WorkSpace** workspaces;
  u32         workspace_count;
  u32         current_workspace;
} WindowManager;

#define NULL ((void*)0)
#define _4B (1 << 2)
#define _4KB (1 << 12)
#define _4MB (1 << 22)

#define stop                                                                   \
  __cli();                                                                     \
  __loop();
