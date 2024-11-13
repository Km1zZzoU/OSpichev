#pragma once
typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef const unsigned int colorType;

#define start_ptr 0xfd000000
#define size_x (16)
#define size_y (24)
#define getptr(x,y, offset) *(u32*) (0xfd000000 + 3 * (x * 16 + w * (y * 24) + offset))
#define bgh 0x1d2021
#define bg0 0x282828
#define bg3 0x665c54
#define bg4 0x7c6f64
#define fg 0xebdbb2
#define red1 0xcc241d
#define red0 0xfb4934
#define green0 0x98971a
#define yellow0 0xd79921
#define orange0 0xfe8019
#define blue1 0x458588

#define w 1600
#define h 1200

#define size_w (w / size_x - 1)
#define size_h (h / size_y - 1)

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