run:
	nasm -fbin src/boot.asm -o boot.bin
	dd if=/dev/zero of=./boot.img bs=1024 count=1440
	dd if=./boot.bin of=./boot.img conv=notrunc
	dd if=./assets/BimBimBamBam.bin of=./boot.img seek=1 conv=notrunc

	qemu-system-x86_64 -fda boot.img -monitor stdio
