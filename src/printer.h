#pragma once
#include "typedef.h"

int curx;
int cury;

void init_printer() {
  vga_clear();
  curx = 0;
  cury = 0;
}

void printf(char* fmt, ...) {
  char *p = fmt;
  int *arg = (int *)&fmt + 1; // Указатель на следующий аргумент

  for (; *p; p++) {
    switch (*p) {
      case '%':
        p++;
        switch (*p) {
        case 's':
          vga_puts((char *)*arg++);
          break;
        case 'h':
          vga_putn(*arg++, 0x10);
          break;
        case 'd':
          vga_putn(*arg++, 10);
          break;
        default:
          vga_putc('%');
          p--;
          break;
        }
        break;
      case '\n':
        for (; curx > 0; vga_putc(' '));
        break;
      default:
        vga_putc(*p);
        break;
    }
  }
}

//сдвигаем весь текст вниз
void shift_down() {
  for (int i = 0; i < size_x; i++) {
    for (int j = 0; j < size_y - 1; j++)
      getptr(i, j, char) = getptr(i, j + 1, char);
  }
}

//печать строки str по глобальным координатам
void vga_puts(char* str) {
  vga_prints(str, &curx, &cury);
}

//пишем с на конкретные координаты, и перемещаем координаты на следующую ячейку с учетом ограниченности экрана
void vga_printc(char c, int* x, int* y) {
  getptr(*x, *y, char) = c;
  if (++*x == size_x) {
    *x = 0;
    if (++*y == size_y) {
      --*y;
      shift_down();
      for (int i = 0; i < size_x; i++)
        getptr(i, *y, char) = 0;
    }
  }
}

//в цикле пишем *str по конкретным координатам
void vga_prints(char* str, int* x, int* y) {
  for (; *str != '\0';)
    vga_printc(*str++, x, y);
}

//печатает символ с по глобальным координатам
void vga_putc(char c) {
  vga_printc(c, &curx, &cury);
}


void vga_putn(int x, int base) {
  if (!x) {
    vga_putc('0');
    return;
  }
  if (x < 0) {
    vga_putc('-');
    x *= -1;
  }
  int chars[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 9; x > 0; x /= base) {
    char digit = x % base;
    if (digit < 10) {
      chars[i--] = digit + '0';
    } else {
      chars[i--] = digit - 10 + 'A';
    }
  }
  for (int i = 0; i < 10; i++) {
    if (chars[i] > 0)
      vga_putc(chars[i]);
  }
}

short get_colar(int n) {
  int max = size_x + size_y - 1;
  int colors = 8;
  int pixel_for_color = max / colors;
  int color = n / pixel_for_color;
  switch (color) {
    case 0:
      return 0x7100;
    case 1:
      return 0x7B00;
    case 2:
      return 0x7300;
    case 3:
      return 0x7900;
    case 4:
      return 0x7500;
    case 5:
      return 0x7D00;
    case 6:
      return 0x7400;
    case 7:
      return 0x7C00;
  }
  return 0;
}

void vga_clear() {
  for (int x = 0; x < size_x; x++) {
    for (int y = 0; y < size_y; y++) {
      getptr(x, y, short) = get_colar(x+y);
      getptr(x, y, short) = 0x0200;
    }
  }
}