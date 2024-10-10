void kernel_entry() {
  *((short int*) 0xb8000) = 0;
  for (;;);
}
