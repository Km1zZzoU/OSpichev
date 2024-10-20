int kmain();
typedef unsigned short u16;
typedef unsigned int u32;
void vga_clear();
void vga_puts(char* c);
void vga_putc(char c);
void vga_putn(int x, int base);
void vga_printc(char c, int* x, int* y);
void vga_prints(char* str, int* x, int* y);
void shift_down();
void init_printer();
void puts(char* fmt, ...);
short get_colar(int n);
static void panic_handler(int vector);
void kpanic(char* msg, int vector);
void* make(u32 size);
int curx = 0, cury = 0;

void __start__() {
  kmain();
}
#define byte unsigned char
#define start_ptr 0xb8000
#define size_x (80)
#define size_y (25)
#define getptr(x,y,t) *((t*) (start_ptr + 2 * ((y) * size_x + (x))))
#define IDT_SIZE 256

#define FUNC_GEN(x) static void trap_##x() { panic_handler(0b##x); }
#define FUNC_GEN_2(b1) FUNC_GEN(b1##0) FUNC_GEN(b1##1)
#define FUNC_GEN_4(b2) FUNC_GEN_2(b2##0) FUNC_GEN_2(b2##1)
#define FUNC_GEN_8(b3) FUNC_GEN_4(b3##0) FUNC_GEN_4(b3##1)
#define FUNC_GEN_16(b4) FUNC_GEN_8(b4##0) FUNC_GEN_8(b4##1)
#define FUNC_GEN_32(b5) FUNC_GEN_16(b5##0) FUNC_GEN_16(b5##1)
#define FUNC_GEN_64(b6) FUNC_GEN_32(b6##0) FUNC_GEN_32(b6##1)
#define FUNC_GEN_128(b7) FUNC_GEN_64(b7##0) FUNC_GEN_64(b7##1)
#define FUNC_GEN_ALL() FUNC_GEN_128(0) FUNC_GEN_128(1)

FUNC_GEN_ALL()

#define FUNC_ASSIGN(x) traps[0b##x] = trap_##x;
#define FUNC_ASSIGN_2(b1) FUNC_ASSIGN(b1##0) FUNC_ASSIGN(b1##1)
#define FUNC_ASSIGN_4(b2) FUNC_ASSIGN_2(b2##0) FUNC_ASSIGN_2(b2##1)
#define FUNC_ASSIGN_8(b3) FUNC_ASSIGN_4(b3##0) FUNC_ASSIGN_4(b3##1)
#define FUNC_ASSIGN_16(b4) FUNC_ASSIGN_8(b4##0) FUNC_ASSIGN_8(b4##1)
#define FUNC_ASSIGN_32(b5) FUNC_ASSIGN_16(b5##0) FUNC_ASSIGN_16(b5##1)
#define FUNC_ASSIGN_64(b6) FUNC_ASSIGN_32(b6##0) FUNC_ASSIGN_32(b6##1)
#define FUNC_ASSIGN_128(b7) FUNC_ASSIGN_64(b7##0) FUNC_ASSIGN_64(b7##1)
#define FUNC_ASSIGN_ALL FUNC_ASSIGN_128(0) FUNC_ASSIGN_128(1)

static void* START = 0x100000;
#define END 0x400000



#pragma pack(push, 1)
typedef struct {
  u16 low_bits;
  u16 segment_selector;
  u16 flags;
  u16 high_bits;
} gdst;

typedef struct {
  u16 size;
  u32 ptr;
} didt;
#pragma pack(push)

int kmain() {
  init_printer();
  void** traps = make(IDT_SIZE * sizeof(void*));

  FUNC_ASSIGN_ALL

  gdst *idt = make(IDT_SIZE * sizeof(gdst));

  for (int vector = 0; vector < IDT_SIZE; vector++) {
    byte* handler = traps[vector];
    u16 low = (u16) handler;
    u16 seg_sel = 8;
    u16 flags = 0x8f00;
    u16 high = (u32) handler >> 16;

    idt[vector].segment_selector = seg_sel;
    idt[vector].low_bits = low;
    idt[vector].flags = flags;
    idt[vector].high_bits = high;
  }
  didt didt = {
  .size = (u16) (sizeof(gdst) * IDT_SIZE - 1),
  .ptr = (u32) idt,
  };

  __asm__ __volatile__("lidt %0" : : "m"(didt));

  int a = 228 / 1;
  puts("\n%d\n", a);
  __asm__ __volatile__(
    "sti\n\t"
    "int %0"
    :
    : "i"(26)
  );
  puts("\n===\n ====\n  =======\n   ===========----################*****++++===\n     ==============              +%%%%%%%%%%%%%%##**+==\n       =================         *%%%%%%%%%%%%%%%%%%%%%#*+=\n     =======================     *%%%%%%%%%%%%%%%%%%%%%%%%%#*=\n   ==----------------------------+############################*=   ====    ====\n  ===============================+##############################++=    =++=    =\n  ==                             *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%++=    =++=    =\n   ========                      +%%%%%%%%%%%%%%%%%%%%%%%%%%%%%+   ====    ====\n     +#%%%%%%####***====++++     *%%%%%%%%%%%%%%%%%%%%%%%%%%#*=\n        =*##%%%%%%%%%%%%%%%%%%%%#*%%%%%%%%%%%%%%%%%%%%%%##+=\n            ==+#************#%%%#*%%%%%%%%%%%%%%%%##*++=\n              +#-=         *#%%%*+#######***+++==\n              #+          ** =*#*     *#*\n             +#          =#=       =+##+\n             #+          *%#**#*###* ____   _____              _\n            *#          =#@=======  / __ \\ / ____|     (*)    | |\n           =#=          *=         | |  | | (___  _ __  _  ___| |__   _____   __\n           **          =#=         | |  | |\\___ \\| ._ \\| |/ __| ._ \\ / _ \\ \\ / /\n          =#=          *=          | |__| |____) | |_) | | (__| | | |  __/\\ V /\n          **          =#=           \\____/ \\____/| .__/|_|\\___|_| |_|\\___| \\_/\n         *#==        =+#=                        | |\n         +*+*+++++++++**=                        |_|");
  for (;;);
}

void* make(u32 size) {
  if ((u32) START + size > (u32) END) {
    kpanic("out of memory", -1);
  }
  void* cur = START;
  START += size;
  return cur;
}

__inline__
void panic_handler(int vector) {
  kpanic("unhandled interrupt %h", vector);
}

void kpanic(char* msg, int vector) {
  init_printer();
  puts("\n  ___                    ");
  puts("\n /   \\                   ");
  puts("\n|     |___________       ");
  puts("\n \\___/          | \\      ");
  puts("\n /   \\__________|_/ **   ");
  puts("\n|     |              **  ");
  puts("\n \\___/                ** \n                ");
  puts(msg, vector);
  for (;;);
}

void init_printer() {
  vga_clear();
  curx = 0;
  cury = 0;
}

void puts(char* fmt, ...) {
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
      getptr(x, y, short) = 0x7c00;
    }
  }
}