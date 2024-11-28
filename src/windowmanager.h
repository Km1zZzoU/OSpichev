#pragma once
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#include "make.h"
#include "panic.h"
#include "printer.h"
#include "symbol.h"
#include "typedef.h"


#define BASE_OUTSIDE_INDENT (size_x >> 2)
#define BAR_WINDOW_INDENT (14)
#define BORDER_SIZE (size_x >> 2)
#define X_INSIDE_INDENT (size_x >> 4)
#define INSIDE_INDENT_BAR X_INSIDE_INDENT
#define X_BORDER_INDENT (BORDER_SIZE + X_INSIDE_INDENT)
#define Y_INSIDE_INDENT_WIN                                                    \
  (((size_y - BASE_OUTSIDE_INDENT - 2 * BORDER_SIZE) >> 1) - 1)
#define Y_BORDER_INDENT_WIN (BORDER_SIZE + Y_INSIDE_INDENT_WIN)
#define BORDER_INDENT_BAR (BORDER_SIZE + INSIDE_INDENT_BAR)
#define BAR_FULL_SIZE (2 * BORDER_INDENT_BAR + size_y)
#define Y_OUTSIDE_INDENT_WIN                                                   \
  (BASE_OUTSIDE_INDENT + BAR_FULL_SIZE + BAR_WINDOW_INDENT)

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
// 5->1+4
// 6->2+4
// 7->1+2+2+2
// 8->1+2+2+3
// 9->1+2+3+3
// 10->1+3+3+3
// 11->1+3+3+4
// 12->1+3+4+4
// 13->1+4+4+4
// 14->2+4+4+4
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

static byte splitn(const byte x, const byte n) {
  return (x - n + 1) / n;
}

static u32 getx0(const byte x) {
  return BASE_OUTSIDE_INDENT + x * size_x;
}

static u32 gety0(const byte y) {
  return Y_OUTSIDE_INDENT_WIN + y * size_y;
}


void clear_workspace() {
  for (u32 y = 0; y < size_h_window * size_y + 2 * Y_BORDER_INDENT_WIN; y++) {
    for (u32 x = 0; x < size_w_window * size_x + 2 * X_BORDER_INDENT; x++)
      getptr(0, 0, x + BASE_OUTSIDE_INDENT + w * (y + Y_OUTSIDE_INDENT_WIN)) =
          0;
  }
}

void resize(WorkSpace* wSpace) {
  clear_workspace();
  for (Window* win = wSpace->window; win; win = win->next)
    update_window(win, 1, 1);
}

static Window* last(Window* window) {
  return window->next ? last(window->next) : window;
}

Window* append_window() {
  WorkSpace* work_space = _wManager.workspaces[_wManager.current_workspace];
  if (!work_space)
    kpanic("WindowManager: %d work space is null", _wManager.current_workspace);
  const byte count = ++work_space->window_count;
  const byte lim   = 14;
  if (count > lim)
    kpanic("WindowManager: %d out of lim windows", count);
  Window* window  = make(sizeof(Window));
  window->symbols = (symbol*)make(START_SIZE_BUFFER_WINDOW);

  window->index_for_show = 0;
  window->size_buff      = 0;

  window->next     = NULL;
  Window* main     = work_space->window;
  last(main)->next = window;

  Window* ws[lim];
  ws[0] = main;
  for (byte i = 1; i < lim; i++) {
    ws[i] = ws[i - 1] ? ws[i - 1]->next : NULL;
  }
  byte count_stacks = count < 7 ? 1 : 3;
  // color_printf(red1, "%d", count_stacks);
  byte count_in_main   = ((count & 7) == 6) + 1;
  byte count_in_stacks = count - count_in_main;

  // weight
  for (byte i = 0; ws[i]; i++) {
    ws[i]->width = splitn(size_w_window, count_stacks + 1);
  }

  byte nearest_mul_of_the_number_of_columns = count_stacks;
  for (; nearest_mul_of_the_number_of_columns < count - count_in_main;
       nearest_mul_of_the_number_of_columns += count_stacks)
    ;
  byte max_count_in_stack = nearest_mul_of_the_number_of_columns / count_stacks;
  // color_printf(orange0, "max_count_in_stack %d\n", max_count_in_stack);
  byte need_to_add = nearest_mul_of_the_number_of_columns - count_in_stacks;

  // x0 mains
  for (byte i = 0; i < count_in_main; i++) {
    ws[i]->x0 = getx0(0);
  }
  // mini main
  if (count_in_main == 2) {
    ws[1]->height = splitn(size_h_window, max_count_in_stack);
    ws[1]->y0     = gety0(size_h_window - ws[1]->height);
    // main height
    main->height = size_h_window - ws[1]->height - 1;
  } else {
    main->height = size_h_window;
  }

  // stack
  byte k = count_in_main;
  for (byte i = 0; i < count_stacks; i++) {
    const byte count_in_cur = max_count_in_stack - (i < need_to_add);
    for (byte j = 0; j < count_in_cur; j++) {
      ws[k]->x0 =
          getx0((splitn(size_w_window, count_stacks + 1) + 1) * (i + 1));
      ws[k]->height = splitn(size_h_window, count_in_cur);

      ws[k]->y0 = gety0((ws[k]->height + 1) * j);
      k++;
    }
  }
  resize(work_space);
  return window;
}

static void init_window(WorkSpace* wSpace) {
  if (!wSpace)
    kpanic("WindowManager: null window space1", -1);

  wSpace->window = (Window*)make(sizeof(Window));

  wSpace->window->symbols        = (symbol*)make(START_SIZE_BUFFER_WINDOW);
  wSpace->window->size_buff      = 0;
  wSpace->window->index_for_show = 0;

  wSpace->window->x0 = BASE_OUTSIDE_INDENT;
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
  const u32 width  = size_x * window->width + 2 * X_BORDER_INDENT;
  const u32 height = size_y * window->height + 2 * Y_BORDER_INDENT_WIN;

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
  for (u32 y = 0; y < window->height * size_y + 2 * Y_INSIDE_INDENT_WIN; y++) {
    for (u32 x = 0; x < window->width * size_x + 2 * X_INSIDE_INDENT; x++)
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
        getptr(x, y,
               (x0 + j + X_BORDER_INDENT +
                w * (y0 + i + Y_BORDER_INDENT_WIN))) = c.color;
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
      START_SIZE_BUFFER_WINDOW / (sizeof(symbol)))
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