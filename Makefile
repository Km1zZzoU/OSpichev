run:
	nasm -fbin helloworld/boot.asm -o boot.bin
	dd if=./boot.bin of=./boot.img conv=notrunc
	qemu-system-x86_64 -hda boot.img -monitor stdio
