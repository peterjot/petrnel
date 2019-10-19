[ORG 0x7C00]

MOV BX, HELLO
CALL print

CALL print_nl

MOV bx, GOODBY
CALL print


CALL print_nl

MOV DX, 0x12FE
CALL print_hex


JMP $

%include "print.asm"
%include "print_hex.asm"

; data

HELLO:
    DB 'Hello, World', 0

GOODBY:
    DB 'Goodbye', 0

TIMES 510-($-$$) DB 0
DW 0xAA55



