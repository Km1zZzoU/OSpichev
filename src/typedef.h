#pragma once
typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;

#define start_ptr 0xb8000
#define size_x (80)
#define size_y (25)
#define getptr(x,y,t) *((t*) (start_ptr + 2 * ((y) * size_x + (x))))
#define IDT_SIZE 256

#define __FUNC_GEN(x) extern void __trap_##x();
#define __FUNC_GEN_2(b1) __FUNC_GEN(b1##0) __FUNC_GEN(b1##1)
#define __FUNC_GEN_4(b2) __FUNC_GEN_2(b2##0) __FUNC_GEN_2(b2##1)
#define __FUNC_GEN_8(b3) __FUNC_GEN_4(b3##0) __FUNC_GEN_4(b3##1)
#define __FUNC_GEN_16(b4) __FUNC_GEN_8(b4##0) __FUNC_GEN_8(b4##1)
#define __FUNC_GEN_32(b5) __FUNC_GEN_16(b5##0) __FUNC_GEN_16(b5##1)
#define __FUNC_GEN_ALL() __FUNC_GEN_32(0) __FUNC_GEN(100000) __FUNC_GEN(100001)

#define __FUNC_ASSIGN(x) traps[0b##x] = __trap_##x;
#define __FUNC_ASSIGN_2(b1) __FUNC_ASSIGN(b1##0) __FUNC_ASSIGN(b1##1)
#define __FUNC_ASSIGN_4(b2) __FUNC_ASSIGN_2(b2##0) __FUNC_ASSIGN_2(b2##1)
#define __FUNC_ASSIGN_8(b3) __FUNC_ASSIGN_4(b3##0) __FUNC_ASSIGN_4(b3##1)
#define __FUNC_ASSIGN_16(b4) __FUNC_ASSIGN_8(b4##0) __FUNC_ASSIGN_8(b4##1)
#define __FUNC_ASSIGN_32(b5) __FUNC_ASSIGN_16(b5##0) __FUNC_ASSIGN_16(b5##1)
#define __FUNC_ASSIGN_ALL() __FUNC_ASSIGN_32(0) __FUNC_ASSIGN(100000) __FUNC_ASSIGN(100001)

#define FUNC_GEN(x) static void trap_##x() { kpanic_handler(0b##x); } __FUNC_GEN(0b##x)
#define FUNC_GEN_2(b1) FUNC_GEN(b1##0) FUNC_GEN(b1##1)
#define FUNC_GEN_4(b2) FUNC_GEN_2(b2##0) FUNC_GEN_2(b2##1)
#define FUNC_GEN_8(b3) FUNC_GEN_4(b3##0) FUNC_GEN_4(b3##1)
#define FUNC_GEN_16(b4) FUNC_GEN_8(b4##0) FUNC_GEN_8(b4##1)
#define FUNC_GEN_32(b5) FUNC_GEN_16(b5##0) FUNC_GEN_16(b5##1)
#define FUNC_GEN_64(b6) FUNC_GEN_32(b6##0) FUNC_GEN_32(b6##1)
#define FUNC_GEN_128(b7) FUNC_GEN_64(b7##0) FUNC_GEN_64(b7##1)
#define FUNC_GEN_ALL() FUNC_GEN_128(0) FUNC_GEN_128(1)

#define FUNC_ASSIGN(x) traps[0b##x] = trap_##x;
#define FUNC_ASSIGN_2(b1) FUNC_ASSIGN(b1##0) FUNC_ASSIGN(b1##1)
#define FUNC_ASSIGN_4(b2) FUNC_ASSIGN_2(b2##0) FUNC_ASSIGN_2(b2##1)
#define FUNC_ASSIGN_8(b3) FUNC_ASSIGN_4(b3##0) FUNC_ASSIGN_4(b3##1)
#define FUNC_ASSIGN_16(b4) FUNC_ASSIGN_8(b4##0) FUNC_ASSIGN_8(b4##1)
#define FUNC_ASSIGN_32(b5) FUNC_ASSIGN_16(b5##0) FUNC_ASSIGN_16(b5##1)
#define FUNC_ASSIGN_64(b6) FUNC_ASSIGN_32(b6##0) FUNC_ASSIGN_32(b6##1)
#define FUNC_ASSIGN_128(b7) FUNC_ASSIGN_64(b7##0) FUNC_ASSIGN_64(b7##1)
#define FUNC_ASSIGN_ALL FUNC_ASSIGN_128(0) FUNC_ASSIGN_128(1)