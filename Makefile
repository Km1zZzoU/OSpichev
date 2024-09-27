run:
	nasm -fbin src/boot.asm -o boot.bin
	dd if=./boot.bin of=./boot.img conv=notrunc
	dd if=src/bimbam.txt of=./boot.img seek=1 conv=notrunc
	qemu-system-x86_64 -hda boot.img -monitor stdio
