all:
	~/opt/cross/bin/i686-elf-as ./boot.S -o boot.o
	~/opt/cross/bin/i686-elf-as ./isr.S -o isr.o
	#~/opt/cross/bin/i686-elf-as ./startup.asm -o startup.o
	nasm -f elf32 ./startup.asm -o startup.o

	~/opt/cross/bin/i686-elf-gcc -c \
		./kernel.c \
		./mem.c \
		./term.c \
		./str.c \
		./task.c \
		./vm.c \
		./vm_ops.c \
		./io.c \
		./fat12.c \
		./gdt.c \
		./idt.c \
		./keybd.c \
		./dasm.c \
		./time.c \
		 -std=gnu99 -ffreestanding -O2 -Wfatal-errors -w

	~/opt/cross/bin/i686-elf-gcc -T linker.ld -o d3.kernel -ffreestanding -O2 -nostdlib \
		boot.o \
		kernel.o \
		mem.o \
		term.o \
		str.o \
		task.o \
		switch.o \
		vm.o \
		vm_ops.o \
		io.o \
		fat12.o \
		gdt.o \
		idt.o \
		keybd.o \
		dasm.o \
		time.o \
		startup.o \
		-lgcc

clean:
	rm *.o
	rm d3.kernel
