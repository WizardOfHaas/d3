all:
	cd src && $(MAKE)

install:
	mount -o loop disk.img ./dsk
	cp src/d3.kernel dsk/
	sleep 1
	umount dsk

clean:
	rm *.o
	rm d3.kernel

	cd src && $(MAKE)

test:
	qemu-system-i386 -fda disk.img
