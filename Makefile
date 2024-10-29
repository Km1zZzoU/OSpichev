SRC_DIR := src

BOOT_BIN := boot.bin
VESA_BIN := vesa.bin
KERNEL_BIN := kernel.bin
BOOT_IMG := boot.img

QEMU := qemu-system-i386

NASM_FLAGS := -f bin
GCC_FLAGS := -m32 -ffreestanding -fno-pic -O0 -mno-sse -fno-stack-protector
LD_FLAGS_KERNEL := -m elf_i386 -T link.lds
DD_FLAGS := conv=notrunc

all: clean $(BOOT_IMG) run

$(BOOT_IMG): $(BOOT_BIN) $(VESA_BIN) $(KERNEL_BIN)
	dd if=/dev/zero of=$@ bs=1024 count=1440

	dd if=$(BOOT_BIN) of=$@ $(DD_FLAGS)

	dd if=$(VESA_BIN) of=$@ $(DD_FLAGS) seek=1

	dd if=$(KERNEL_BIN) of=$@ $(DD_FLAGS) seek=2

$(BOOT_BIN): $(SRC_DIR)/boot.asm
	nasm $(NASM_FLAGS) $< -o $@

$(VESA_BIN): $(SRC_DIR)/vesa.asm
	nasm $(NASM_FLAGS) $< -o $@

$(KERNEL_BIN): $(SRC_DIR)/kernel.c
	gcc $(GCC_FLAGS) -c $< -o kernel.o
	ld $(LD_FLAGS_KERNEL) kernel.o -o $@

run: $(BOOT_IMG)
	$(QEMU) -fda $(BOOT_IMG) -monitor stdio

gdb: $(BOOT_IMG)
	$(QEMU) -fda $(BOOT_IMG) -monitor stdio -s -S

clean:
	rm -f $(BOOT_BIN) $(KERNEL_BIN) kernel.o $(BOOT_IMG) dump.bin

.PHONY: all clean run
