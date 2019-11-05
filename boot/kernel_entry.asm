[bits 32]
[extern main] ; Define calling point
call main ; call the C function. The linker will know where it is placed in memory
jmp $
