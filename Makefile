SRC_DIR := src

BOOT_BIN := boot.bin
KERNEL_BIN := kernel.bin
BOOT_IMG := boot.img

QEMU := qemu-system-i386

BOOT_FLAGS := -f bin
NASM_FLAGS := -f elf
GCC_FLAGS := -m32 -ffreestanding -fno-pic -O0 -mno-sse -fno-stack-protector
LD_FLAGS_KERNEL := -m elf_i386 -T link.lds
DD_FLAGS := conv=notrunc

all: clean run

$(BOOT_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	dd if=/dev/zero of=$@ bs=1024 count=1440

	dd if=$(BOOT_BIN) of=$@ $(DD_FLAGS)

	dd if=$(KERNEL_BIN) of=$@ $(DD_FLAGS) seek=1

$(BOOT_BIN): boot/boot.asm
	nasm $(BOOT_FLAGS) $< -o $@

$(KERNEL_BIN): $(SRC_DIR)/kernel.c utils
	gcc $(GCC_FLAGS) -c $< -o kernel.o
	ld $(LD_FLAGS_KERNEL) *.o -o $@

utils:
	nasm $(NASM_FLAGS) $(SRC_DIR)/utils.asm -o utils.o

run: $(BOOT_IMG)
	$(QEMU) -fda $(BOOT_IMG) -monitor stdio

gdb: $(BOOT_IMG)
	$(QEMU) -fda $(BOOT_IMG) -monitor stdio -s -S

clean:
	rm -f $(BOOT_BIN) $(KERNEL_BIN) *.o $(BOOT_IMG) *.bin

.PHONY: all clean run
