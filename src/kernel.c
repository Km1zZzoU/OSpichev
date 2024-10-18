#define start_ptr 0xb8000
#define size_x (80)
#define size_y (25)

#define u8 unsigned char
#define u16 unsigned short
typedef void (*interrupt_handler_t)(void);

  /******************/
  /* MY FUNCTIONS */
/*****************/
void vga_clear();
void shift_down();
void init_printer();
void vga_putc(char c);
void vga_puts(char *c);
void printf(char *fmt, ...);
void vga_putn(int x, int base);
void vga_printc(char c, int *x, int *y);
void vga_prints(char *str, int *x, int *y);
short get_colar(int n);
// ──────▄▀▄─────▄▀▄
// ─────▄█░░▀▀▀▀▀░░█▄
// ─▄▄──█░░░░░░░░░░░█──▄▄
// █▄▄█─█░░▀░░┬░░▀░░█─█▄▄█
//────────────────────────
void __pic_remap();                                           // Переназначение контроллера прерываний (PIC)
void __pic_eoi(u8 irq);                                       // Отправка сигнала завершения прерывания (EOI)
void __interrupt_handler(u8 n, interrupt_handler_t handler);  // Регистрация обработчика прерывания
void __init_interrupts();
void __kb_handler();
void __irq_handler();
void __init_kb();

  /************************/
 /* My Port and PIC Info */
/************************/
#define KB_DATA_PORT 0x60
#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA    0x21
#define PIC2_DATA    0xA1
#define PIC_EOI      0x20       // Код завершения прерывания (End of Interrupt)
#define IRQ1         33         // Номер IRQ для клавиатуры

  /*****************************/
 /* My Interrupt Vector Info  */
/*****************************/
#define ICW1 0x11                // ICW1: Инициализация контроллера прерываний
#define ICW2_PIC1 0x20           // Вектор для IRQ0-7 (первый PIC)
#define ICW2_PIC2 0x28           // Вектор для IRQ8-15 (второй PIC)

// давай дружить
#define ICW3_PIC1 0x04
#define ICW3_PIC2 0x02

#define ICW4 0x01

#define getptr(x, y, t) *((t *)(start_ptr + 2 * ((y) * size_x + (x))))

int curx = 0, cury = 0;

interrupt_handler_t interrupt_handlers[256];

int kmain() {
  //return 0;
  init_printer();
  for (int i = 0; i < 600; i++) {
    printf("######", i);
  }
  printf("   Hello World! \n");
  // printf("Hello World! can u solve this?: %s-%d+%h, btw best bytes it is %h
  // also i hate %h%h", msg, 450, 0x7c00, 0x55AA, 0xCAFE, 0xBABE);
  // printf("\n===\n ====\n  =======\n
  // ===========----################*****++++===\n     ==============
  // +%%%%%%%%%%%%%%##**+==\n       =================
  // *%%%%%%%%%%%%%%%%%%%%%#*+=\n     =======================
  // *%%%%%%%%%%%%%%%%%%%%%%%%%#*=\n
  // ==----------------------------+############################*=   ==== ====\n
  // ===============================+##############################++=    =++=
  // =\n  ==                             *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%++= =++=
  // =\n   ========                      +%%%%%%%%%%%%%%%%%%%%%%%%%%%%%+   ====
  // ====\n     +#%%%%%%####***====++++     *%%%%%%%%%%%%%%%%%%%%%%%%%%#*=\n
  // =*##%%%%%%%%%%%%%%%%%%%%#*%%%%%%%%%%%%%%%%%%%%%%##+=\n
  // ==+#************#%%%#*%%%%%%%%%%%%%%%%##*++=\n              +#-=
  // *#%%%*+#######***+++==\n              #+          ** =*#*     *#*\n +# =#=
  // =+##+\n             #+          *%#**#*###* ____   _____              _\n
  // *#          =#@=======  / __ \\ / ____|     (*)    | |\n           =#= *=
  // | |  | | (___  _ __  _  ___| |__   _____   __\n           **          =#=
  // | |  | |\\___ \\| ._ \\| |/ __| ._ \\ / _ \\ \\ / /\n          =#= *= |
  // |__| |____) | |_) | | (__| | | |  __/\\ V /\n          **          =#=
  // \\____/ \\____/| .__/|_|\\___|_| |_|\\___| \\_/\n         *#==        =+#=
  // | |\n         +*+*+++++++++**=                        |_|"); for (int i =
  // 0;; i++) {
  //   printf("%h", (i << 2 / 3 * 124887042 + 2 * (i << 2 / 3 * 124887042)) >>
  //   i);
  // }
  // init_printer();
  // printf("%h", get_colar(43));
  __pic_remap();

  for (int i = 0;i < 256; interrupt_handlers[i++] = 0);

  __init_interrupts();

  for (;;) {
    vga_putc('!');
    __asm__ __volatile__("hlt");
  }
}

void outb(unsigned short port, u8 value) {
  __asm__ __volatile__ (
      "outb %0, %1" : : "a"(value), "Nd"(port)
  );
}

unsigned char inb(u16 port) {
  u8 ret;
  __asm__ __volatile__ (
      "inb %1, %0" : "=a"(ret) : "Nd"(port)
  );
  return ret;
}


u8 scancode[] = {
  // 0x00 - 0x1F (Контрольные символы)
  0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
  0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
  0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
  '*', 0, ' ', // 0x20 - пробел
  // 0x21 - 0x2F (Специальные символы)
  '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
  // 0x30 - 0x39 (Цифры)
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  // 0x3A - 0x40 (Специальные символы)
  ':', ';', '<', '=', '>', '?', '@',
  // 0x41 - 0x5A (Большие буквы)
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  // 0x5B - 0x60 (Специальные символы)
  '[', '\\', ']', '^', '_', '`',
  // 0x61 - 0x7A (Строчные буквы)
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  // 0x7B - 0x7E (Специальные символы)
  '{', '|', '}', '~', 0,
  // 0x7F - 0xFF (Дополнительные символы CP437)
  // Графические символы и специальные символы:
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x80 - 0x8F
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x90 - 0x9F
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xA0 - 0xAF
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xB0 - 0xBF
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xC0 - 0xCF
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xD0 - 0xDF
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xE0 - 0xEF
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // 0xF0 - 0xFF
};


void __init_interrupts() {
  printf("init interrupts\n");

  printf("running pic_remap...\n");
  __pic_remap();

  printf("running init_kb...\n");
  __init_kb();
}

void __pic_remap() {
  u8 a1;
  u8 a2;

  printf("first inb start...\n");

  a1 = inb(PIC1_DATA);
  a2 = inb(PIC2_DATA);

  printf("first inb complete...\n");
  printf("first outb start...\n");

  outb(PIC1_COMMAND, a1);
  outb(PIC2_COMMAND, a2);

  printf("first outb complete...\n");
  printf("2 outb start...\n");

  outb(PIC1_DATA, ICW2_PIC1); // IRQ0-7
  outb(PIC2_DATA, ICW2_PIC2); // IRQ8-15

  printf("2 outb complete...\n");
  printf("3 outb start...\n");

  //налаживаем свзязь между этими ублюдками
  outb(PIC1_DATA, ICW3_PIC1);
  outb(PIC2_DATA, ICW3_PIC2);

  printf("3 outb complete...\n");
  printf("4 outb start...\n");

  outb(PIC1_DATA, ICW4); // Завершение настройки первого PIC
  outb(PIC2_DATA, ICW4); // Завершение настройки второго PIC

  printf("4 outb complete...\n");
  printf("last outb start...\n");

  //восстанавливаем a*
  outb(PIC1_DATA, a1);
  outb(PIC2_DATA, a2);

  printf("last outb complete...\n");
}

void __init_kb() {
  printf("starting init_kb...\n");
  __interrupt_handler(IRQ1, __kb_handler);
}

void __kb_handler() {
  printf("starting kb_handler...\n");
  u8 code = inb(KB_DATA_PORT);
  vga_putc('1');
  if (code < 0x80) {
    vga_putc('2');
    u8 symbol = scancode[code];
    vga_putc(symbol);
    if (symbol)
      vga_putc(symbol);
  }
  vga_putc('3');
  __pic_eoi(1);
}

void __pic_eoi(u8 irq) {
  if (irq >= 8) {
    outb(PIC2_COMMAND, PIC_EOI);
  }
  outb(PIC1_COMMAND, PIC_EOI);
}

void __interrupt_handler(u8 n, interrupt_handler_t handler) {
  printf("%d %h, %s\n", n, n, n);
  printf("starting interrupt handler\n");
  printf("%h\n", handler);
  interrupt_handlers[n] = handler;
  printf("ending interrupt handler\n");
}

void printf(char *fmt, ...) {
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
      for (; curx > 0; vga_putc(' '))
        ;
      break;
    default:
      vga_putc(*p);
      break;
    }
  }
}

// сдвигаем весь текст вниз
void shift_down() {
  for (int i = 0; i < size_x; i++) {
    for (int j = 0; j < size_y - 1; j++)
      getptr(i, j, char) = getptr(i, j + 1, char);
  }
}

// печать строки str по глобальным координатам
void vga_puts(char *str) {
  vga_prints(str, &curx, &cury);
}

// пишем с на конкретные координаты, и перемещаем координаты на следующую ячейку
// с учетом ограниченности экрана
void vga_printc(char c, int *x, int *y) {
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
// в цикле пишем *str по конкретным координатам
void vga_prints(char *str, int *x, int *y) {
  for (; *str != '\0';)
    vga_printc(*str++, x, y);
}

// печатает символ с по глобальным координатам
void vga_putc(char c) {
  vga_printc(c, &curx, &cury);
}

void init_printer() {
  vga_clear();
  curx = 0;
  cury = 0;
}

void vga_putn(int x, int base) {
  if (x < 0) {
    vga_putc('-');
    x *= -1;
  }
  int chars[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 9; x > 0; x /= base) {
    char digit = x % base;
    if (digit < 10) {
      chars[i] = digit + '0';
    } else {
      chars[i] = digit - 10 + 'A';
    }
    i--;
  }
  for (int i = 0; i < 10; i++) {
    if (chars[i] != 0)
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
    return 0x100;
  case 1:
    return 0xB00;
  case 2:
    return 0x300;
  case 3:
    return 0x900;
  case 4:
    return 0x500;
  case 5:
    return 0xD00;
  case 6:
    return 0x400;
  case 7:
    return 0xC00;
  }
  return 0;
}

void vga_clear() {
  for (int x = 0; x < size_x; x++) {
    for (int y = 0; y < size_y; y++)
      getptr(x, y, short) = get_colar(x+y);
  }
}
