

all: run


os-image: asm/bootsector.asm
	nasm -f bin -i asm/ $^ -o os-image

run: os-image
	qemu-system-x86_64 $^ --nographic
