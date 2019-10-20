[ORG 0x7C00]

mov bx, START_MSG
call print
call print_nl

mov bx, DISK_LOAD_MSG
call print
call print_nl

mov bp, 0x8000 ; set stack
mov sp, bp

mov bx, 0x9000 ; set pointer buffer 
mov dh, 2      ; bios sets 'dl' for boot disk num
call disk_load


mov bx, DISK_LOADED_MSG
call print
call print_nl


mov dx, [0x9000]
call print_hex
call print_nl

mov dx, [0x9000 + 512] ; first word from second sector
call print_hex

JMP $

%include "print.asm"
%include "print_hex.asm"
%include "disk.asm"

; data

START_MSG: db "Starting bootlodaer", 0
DISK_LOAD_MSG: db "Loading disk", 0 
DISK_LOADED_MSG: db "Loaded disk", 0

TIMES 510-($-$$) DB 0
DW 0xAA55

times 256 dw 0xdada
times 256 dw 0xface
