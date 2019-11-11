ARCH = i386
QEMU = qemu-system-$(ARCH)
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

OBJ = ${C_SOURCES:.c=.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g

all: run

cleanrun: clean run

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: boot/bootsector.bin kernel.bin boot/empty.bin
	cat $^ > os-image.bin

build: os-image.bin

run: os-image.bin
	$(QEMU) --curses -hda $<

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

debug: os-image.bin kernel.elf
	$(QEMU) --curses -s -hda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -Wall -Wextra -Werror -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o
