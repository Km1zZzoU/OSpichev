#pragma once
#include "externs.h"
#include "kernel.h"
#include "printer.h"
#include "symbol.h"
#include "typedef.h"

byte scancode[] = {
    // 0x00 - 0x1F (Контрольные символы)
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9',  '0', '-', '=',  0,
    0,   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  '[', ']', 0,    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*',  0,   ' ', // 0x39
};

byte shift_scancode[] = {
    // 0x00 - 0x1F (Контрольные символы)
    0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,   0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,   '|', 'Z',
    'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', // 0x39
};

void kb_trap() {
  click_handler();
}

byte shift_togle = 0;

void click_handler(Window* window) {
  u16 kbd_port = 0x60;

  byte symbol = __inb(kbd_port);
  if (symbol == 0xAA || symbol == 0xB6)
    shift_togle = 0;
  if (symbol < 0x40) {
    switch (symbol) {
      case 0x1C:
        __puts(window, "\n");
        break;

      case 0x0E:
        window->symbols[--window->size_buff].character = 0;
        break;

      case 0xF:
        break;
        if (curx == size_w - 2)
          vga_putc(fg, ' ');
        vga_putc(fg, ' ');
        vga_putc(fg, ' ');
        break;

      case 0x38: // alt
        break;

      case 0x2A:
      case 0x36:
        shift_togle = 1;
        break;
      default:
        if (shift_togle) {
          __puts(window, shift_scancode + symbol);
        } else {
          __puts(window, scancode + symbol);
        }
        break;
    }
  }
}