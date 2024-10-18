SRC_DIR := src

BOOT_BIN := boot.bin
KERNEL_BIN := kernel.bin
BOOT_IMG := boot.img

QEMU := qemu-system-i386

NASM_FLAGS := -fbin
GCC_FLAGS := -m32 -ffreestanding -fno-pic -mno-sse -fno-stack-protector
LD_FLAGS_KERNEL := -m elf_i386 -Ttext 0x14200 --oformat binary
DD_FLAGS := conv=notrunc

all: $(BOOT_IMG) run

$(BOOT_IMG): $(BOOT_BIN) $(KERNEL_BIN) $(PROGRAM_BIN)
# Создание пустого образа дискеты
	dd if=/dev/zero of=$@ bs=1024 count=1440

# Запись загрузочного сектора
	dd if=$(BOOT_BIN) of=$@ $(DD_FLAGS)

# Запись ядра
	dd if=$(KERNEL_BIN) of=$@ $(DD_FLAGS) seek=1

$(BOOT_BIN): $(SRC_DIR)/boot.asm
# Ассемблирование boot сектора
	nasm $(NASM_FLAGS) $< -o $@

$(KERNEL_BIN): $(SRC_DIR)/kernel.c
# Компиляция и компоновка ядра
	gcc $(GCC_FLAGS) -c -o kernel.o $<
	ld $(LD_FLAGS_KERNEL) -o $@ kernel.o

run: $(BOOT_IMG)
# Запуск образа в QEMU
	$(QEMU) -m 512 -fda $< -monitor stdio

clean:
# Удаление временных файлов
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(PROGRAM_BIN) kernel.o $(BOOT_IMG) dump.bin

.PHONY: all clean run
