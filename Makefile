run:
	nasm -fbin src/boot.asm -o boot.bin
	dd if=/dev/zero of=./boot.img bs=1024 count=1440
	dd if=./boot.bin of=./boot.img conv=notrunc
	gcc -m32 -ffreestanding -fno-pic -c -o kernel.o src/kernel.c
	ld -m elf_i386 -Ttext 0x14200 --oformat binary -e kernel_entry -o kernel.bin kernel.o
	dd if=kernel.bin of=boot.img conv=notrunc seek=1
	qemu-system-i386 -fda boot.img -monitor stdio