[ORG 0x7C00]

mov bx, MSG_REAL_MODE
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

call switch_to_pm
jmp $

%include "print.asm"
%include "print_hex.asm"
%include "disk.asm"
%include "gdt.asm"
%include "32bit-switch.asm"
%include "32bit-print.asm"

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    jmp $

; data

DISK_LOAD_MSG: db "Loading disk", 0 
DISK_LOADED_MSG: db "Loaded disk", 0
MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loeaded 32-bit protected mode. POZDERKI :)", 0

TIMES 510-($-$$) DB 0
DW 0xAA55

times 256 dw 0xdada
times 256 dw 0xface
