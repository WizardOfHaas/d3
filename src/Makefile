all:
	~/opt/cross/bin/i686-elf-as ./boot.S -o boot.o
	~/opt/cross/bin/i686-elf-gcc -c \
		./kernel.c \
		./mem.c \
		./term.c \
		./str.c \
		 -std=gnu99 -ffreestanding -O2
	~/opt/cross/bin/i686-elf-gcc -T linker.ld -o d3.kernel -ffreestanding -O2 -nostdlib \
		boot.o \
		kernel.o \
		mem.o \
		term.o \
		str.o \
		-lgcc

clean:
	rm *.o
	rm d3.kernel