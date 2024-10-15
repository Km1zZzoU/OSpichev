void vga_clear();
void vga_puts(char* c);
void vga_putc(char c);
void vga_putn(int x, int base);
void vga_printc(char c, int* x, int* y);
void vga_prints(char* str, int* x, int* y);
void shift_down();
void init_printer();
void printf(char* fmt, ...);

#define start_ptr 0xb8000
#define size_x (80)
#define size_y (25)
#define infinite_loop for (;;);
#define getptr(x,y,t) *((t*) (start_ptr + 2 * ((y) * size_x + (x))))

int curx = 0, cury = 0;

int kmain() {
  vga_clear();
  char msg[4] = "123";
  for (int i = 0; i < 600; i++) {
    printf("%d ", i);
  }
  printf("Hello World! can u solve this?: %s-%d+%h, btw best bytes it is %h also i hate %h%h", msg, 456, 0x7c00, 0x55AA, 0xCAFE, 0xBABE);
  printf("\n===\n ====\n  =======\n   ===========----################*****++++===\n     ==============              +%%%%%%%%%%%%%%##**+==\n       =================         *%%%%%%%%%%%%%%%%%%%%%#*+=\n     =======================     *%%%%%%%%%%%%%%%%%%%%%%%%%#*=\n   ==----------------------------+############################*=   ====    ====\n  ===============================+##############################++=    =++=    =\n  ==                             *%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%++=    =++=    =\n   ========                      +%%%%%%%%%%%%%%%%%%%%%%%%%%%%%+   ====    ====\n     +#%%%%%%####***====++++     *%%%%%%%%%%%%%%%%%%%%%%%%%%#*=\n        =*##%%%%%%%%%%%%%%%%%%%%#*%%%%%%%%%%%%%%%%%%%%%%##+=\n            ==+#************#%%%#*%%%%%%%%%%%%%%%%##*++=\n              +#-=         *#%%%*+#######***+++==\n              #+          ** =*#*     *#*\n             +#          =#=       =+##+\n             #+          *%#**#*###* ____   _____              _\n            *#          =#@=======  / __ \\ / ____|     (*)    | |\n           =#=          *=         | |  | | (___  _ __  _  ___| |__   _____   __\n           **          =#=         | |  | |\\___ \\| ._ \\| |/ __| ._ \\ / _ \\ \\ / /\n          =#=          *=          | |__| |____) | |_) | | (__| | | |  __/\\ V /\n          **          =#=           \\____/ \\____/| .__/|_|\\___|_| |_|\\___| \\_/\n         *#==        =+#=                        | |\n         +*+*+++++++++**=                        |_|");
  //for (int i = 0;; i++) {
  //  printf("%h", (i << 2 / 3 * 124887042 + 2 * (i << 2 / 3 * 124887042)) >> i);
  //}
  infinite_loop
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

void vga_clear() {
  for (int x = 0; x < size_x; x++) {
    for (int y = 0; y < size_y; y++)
      getptr(x, y, short) = 0x0A00;
  }
}
