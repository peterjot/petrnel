[ORG 0x7C00]
KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl
mov bp, 0x9000 ; set stack
mov sp, bp

;mov bx, KERNEL_OFFSET ; set pointer buffer 
;mov dh, 2      ; bios sets 'dl' for boot disk num
;mov dl, [BOOT_DRIVE]
;call disk_load
call xx_load_disk

mov bx, DISK_LOADED_MSG
call print
call print_nl


call switch_to_pm
jmp $

%include "boot/print.asm"
%include "boot/print_hex.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/32bit-switch.asm"
%include "boot/32bit-print.asm"

[bits 16]
xx_load_disk:
    mov bx, KERNEL_OFFSET
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET
    jmp $

; data

BOOT_DRIVE db 0
DISK_LOAD_MSG db "Loading disk", 0 
DISK_LOADED_MSG db "Loaded disk", 0
MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loeaded 32-bit protected mode. POZDERKI :)", 0

TIMES 510-($-$$) DB 0
DW 0xAA55
