

all: run


os-image: asm/bootsector.asm
	nasm -f bin -i asm/ $^ -o $@

run: os-image
	qemu-system-x86_64 -fda $^ --nographic
