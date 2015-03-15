all:
	~/opt/cross/bin/i686-elf-as ./src/boot.S -o boot.o
	~/opt/cross/bin/i686-elf-gcc -c \
		./src/kernel.c \
		./src/mem.c \
		./src/term.c \
		 -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	~/opt/cross/bin/i686-elf-gcc -T linker.ld -o d3.kernel -ffreestanding -O2 -nostdlib \
		boot.o \
		kernel.o \
		mem.o \
		term.o \
		-lgcc

install:
	mount -o loop disk.img dsk
	cp d3.kernel dsk/
	umount dsk

clean:
	rm *.o
	rm d3.kernel