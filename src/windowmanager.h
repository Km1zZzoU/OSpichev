#pragma once
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#include "make.h"
#include "panic.h"
#include "printer.h"
#include "symbol.h"
#include "typedef.h"

#define X_OUTSIDE_INDENT (size_x >> 2)
#define Y_OUTSIDE_INDENT (10)
#define BORDER_SIZE (size_x >> 2)
#define INSIDE_INDENT (size_x >> 4)
#define BORDER_INDENT (BORDER_SIZE + INSIDE_INDENT)
#define BETWEEN_INDENT_X (2 * BORDER_INDENT + X_OUTSIDE_INDENT)
#define BETWEEN_INDENT_Y (2 * BORDER_INDENT + Y_OUTSIDE_INDENT)
#define X_INDENT (X_OUTSIDE_INDENT + BORDER_INDENT)
#define Y_INDENT_BAR (Y_OUTSIDE_INDENT + BORDER_INDENT)
#define Y_INDENT_WIN (Y_INDENT_BAR + size_y + BETWEEN_INDENT_Y)
#define Y_OUTSIDE_INDENT_WIN                                                   \
  (Y_INDENT_BAR + size_y + BORDER_INDENT + Y_OUTSIDE_INDENT)

#define size_w_window 99
#define size_h_window 47

#define START_SIZE_BUFFER_WINDOW _4MB

#define border_active_color orange1
#define border_inactive_color bg3
#define bg_window_color bg1

//    0.25 0.25 1/16 49 1/16 0.25 0.25 0.25 1/16 49 1/16 0.25 0.25
//         __________________________________________________ 10
//        |                                                 | 4
//        | .____________________________________________.  | 2
//        | |                                            |  |
//        | |____________________________________________|  | 2
//        |                                                 | 4
//        | .____________________.  .____________________.  | 10
//        | |                    |  |                    |  | 4
//        | |                    |  |                    |  | 2
//        | |                    |  |                    |  |
//        | |                    |  |                    |  |
//        | |                    |  |                    |  | 47*24
//        | |                    |  |                    |  |
//        | |                    |  |                    |  |
//        | |                    |  |                    |  |
//        | |____________________|  |____________________|  | 2
//        |                                                 | 4
//        |_________________________________________________| 4

WindowManager _wManager;

void update_window(Window* window, const int refocus, const int fill);

// 2->1+1
// 3->1+2
// 4->1+3
// 5->2+3
// 6->2+4
// 7->1+2+2+2
//
//
//
Window* focus_window = NULL;

static int active(const Window* window) {
  return window == focus_window;
}

static void active_window(Window* window) {
  const Window* update = focus_window;
  focus_window         = window;
  if (update) {
    update_window(update, 1, 0);
  }
}

static byte split2(const byte x) {
  return (x - 1) >> 1;
}

Window* append_window() {
  WorkSpace* work_space = _wManager.workspaces[_wManager.current_workspace];
  if (!work_space)
    kpanic("WindiwManager: %d work space is null", _wManager.current_workspace);
  const byte count = ++work_space->window_count;

  Window* window  = make(sizeof(Window));
  window->symbols = (symbol*)make(START_SIZE_BUFFER_WINDOW);

  window->index_for_show = 0;
  window->size_buff      = 0;

  window->next = NULL;
  Window* main = work_space->window;

  if (count == 2) {
    main->width = split2(main->width);
    update_window(main, 1, 1);
  }

  switch (count) {
    case 2:
      // __sti();
      main->next     = window;
      window->y0     = main->y0;
      window->height = main->height;
      window->x0     = (w + X_OUTSIDE_INDENT) / 2;
      window->width  = main->width;
      break;
    default:
      break;
  }

  update_window(window, 1, 1);
  return window;
}

static void init_window(WorkSpace* wSpace) {
  if (!wSpace)
    kpanic("WindowManager: null window space1", -1);

  wSpace->window = (Window*)make(sizeof(Window));

  wSpace->window->symbols        = (symbol*)make(_4KB);
  wSpace->window->size_buff      = 0;
  wSpace->window->index_for_show = 0;

  wSpace->window->x0 = X_OUTSIDE_INDENT;
  wSpace->window->y0 = Y_OUTSIDE_INDENT_WIN;

  wSpace->window->width  = size_w_window;
  wSpace->window->height = size_h_window;
  wSpace->window->next   = NULL;
}

static void init_workspaces() {
  for (u32 i = 0; i < _wManager.workspace_count;) {
    WorkSpace* wSpace = make(sizeof(WorkSpace));

    wSpace->id = i;

    wSpace->window_count   = 1;
    wSpace->current_window = 0;

    init_window(wSpace);

    _wManager.workspaces[i++] = wSpace;
  }
}

Window* init_window_manager(const u32 space_count) {
  _wManager.workspace_count = space_count;
  _wManager.workspaces = (WorkSpace**)make(space_count * sizeof(WorkSpace*));
  _wManager.current_workspace = 0;
  init_workspaces();

  return _wManager.workspaces[_wManager.current_workspace]->window;
}

static void refocus_window(const Window* window) {
  const u32 width  = size_x * window->width + 2 * BORDER_INDENT;
  const u32 height = size_y * window->height + 2 * BORDER_INDENT;

  const colorType color =
      active(window) ? border_active_color : border_inactive_color;


  for (u32 y = 0; y < height; y++) {
    for (u32 x = 0; x < BORDER_SIZE; x++)
      getptr(0, 0, window->x0 + x + w * (y + window->y0)) = color;
    for (u32 x = width - BORDER_SIZE; x < width; x++)
      getptr(0, 0, window->x0 + x + w * (y + window->y0)) = color;
  }

  for (u32 x = 0; x < width; x++) {
    for (u32 y = 0; y < BORDER_SIZE; y++)
      getptr(0, 0, window->x0 + x + w * (y + window->y0)) = color;
    for (u32 y = height - BORDER_SIZE; y < height; y++)
      getptr(0, 0, window->x0 + x + w * (y + window->y0)) = color;
  }
}

static void fill_window(const Window* window) {
  for (u32 y = 0; y < window->height * size_y + 2 * INSIDE_INDENT; y++) {
    for (u32 x = 0; x < window->width * size_x + 2 * INSIDE_INDENT; x++)
      getptr(0, 0,
             window->x0 + BORDER_SIZE + x +
                 w * (window->y0 + BORDER_SIZE + y)) = bg_window_color;
  }
}

static void write_in_window(const u32 x0, const u32 y0, const byte x,
                            const byte y, const symbol c) {
  for (int i = 0; i < size_y; i++) {
    for (int j = 0; j < size_x; j++) {
      const u16 mask = 0b1000000000000000 >> j;

      if ((font[c.character][i] & mask) > 0) {
        getptr(x, y, (x0 + j + BORDER_INDENT + w * (y0 + i + BORDER_INDENT))) =
            c.color;
      }
    }
  }
}

static u32 find_next_index_on_new_line(const Window* window) {
  byte counter;
  for (counter = 0;
       counter < window->width && window->symbols[counter].character != '\n';
       counter++)
    ;
  return window->index_for_show + counter;
}

void update_window(Window* window, const int refocus, const int fill) {
  if (refocus)
    refocus_window(window);
  if (fill)
    fill_window(window);

  byte symbol_x = 0;
  byte symbol_y = 0;

  for (u32 i = window->index_for_show; window->symbols[i].character; i++) {

    if (symbol_x == window->width) {
      --i;
      goto L1;
    }

    if (window->symbols[i].character == '\n') {
    L1:
      symbol_y++;
      symbol_x = 0;
      if (symbol_y == window->height) {
        window->index_for_show = find_next_index_on_new_line(window);
        update_window(window, 0, 1);
        return;
      }
    } else {
      write_in_window(window->x0, window->y0, symbol_x++, symbol_y,
                      window->symbols[i]);
    }
  }
}

u32 ko_out(Window* window, const symbol* kostr) {
  if (!window)
    kpanic("WindowManager: ko_out in NULL window", -1);

  if (window->size_buff + kolen(kostr) >
      START_SIZE_BUFFER_WINDOW / sizeof(symbol))
    kpanic("WindowManager: ko_out in too many symbols", -1);

  active_window(window);
  window->size_buff += kostr_copy(window->symbols + window->size_buff, kostr);

  update_window(window, 1, 0);
  // free(kostr);


  return window->size_buff;
}

u32 col_c_out(Window* window, const char* cstr, const colorType color) {
  if (!window)
    kpanic("WindowManager: col_c_out in NULL window", -1);

  symbol* kostr = fill_kostring(cstr, color);
  return ko_out(window, kostr);
}

u32 c_out(Window* window, const char* cstr) {
  if (!window)
    kpanic("WindowManager: c_out in NULL window", -1);

  return col_c_out(window, cstr, fg);
}