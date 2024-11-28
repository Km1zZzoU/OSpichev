#pragma once
#include "externs.h"
#include "kernel.h"
#include "printer.h"
#include "symbol.h"
#include "typedef.h"
#include "windowmanager.h"

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

static char* get_str_for_out(byte* scancode, const byte c) {
  char* str = make(2 * sizeof(byte));
  str[0]    = scancode[c];
  str[1]    = 0;
  return str;
}

void kb_trap(Window* window) {
  click_handler(window);
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
        c_out(window, "\n");
        break;

      case 0x0E:
        window->symbols[--window->size_buff].character = 0;
        update_window(window, 0, 1);
        break;

      case 0xF: // tab
        break;

      case 0x38: // alt
        break;

      case 0x2A:
      case 0x36:
        shift_togle = 1;
        break;
      default:
        if (shift_togle) {
          c_out(window, get_str_for_out(shift_scancode, symbol));
        } else {
          c_out(window, get_str_for_out(scancode, symbol));
        }
        break;
    }
  }
}