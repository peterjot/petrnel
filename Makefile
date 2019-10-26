ARCH=x86_64
QEMU = qemu-system-$(ARCH)


all: run


os-image.bin: asm/bootsector.asm
	nasm -f bin -i asm/ $^ -o $@

build: os-image.bin

run: os-image.bin
	$(QEMU) --curses -drive format=raw,file=$^

clean:
	@for f in `find . -name "*.bin" -o -name "*.o"`; do\
		rm $$f; \
	done
