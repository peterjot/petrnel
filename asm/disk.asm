; input: 'dh' - num of sectors, 'dl' - drive number, '[es:bx]' - pointer to buffer
disk_load:
    pusha
    ;http://stanislavs.org/helppc/int_13-2.html
    push dx

    mov ah, 0x02 ; 0x02 = 'read function id'
    mov al, dh   ; sectors to read
    mov cl, 0x02 ; start from second sector -> skip boot sector
    mov ch, 0x00 ; cilinder num
    ;  'dl' - drive number, by default set by bios, or by caller
    mov dh, 0x00 ; head num
    ; read to [es:bx]
    int 0x13     ; bios interrupt

    jc disk_error
    
    pop dx
    cmp al, dh ; check read num of sectors, bios sets num of sectors read to 'al'
    jne sector_error

    popa
    ret


disk_error:
    mov bx, DISK_ERR_MSG
    call print
    call print_nl

    mov dh, ah ; error code
    call print_hex
    call print_nl

    jmp $

sector_error:
    mov bx, SECTOR_ERR_MSG
    call print
    call print_nl

DISK_ERR_MSG: db "Cannot read from disk", 0
SECTOR_ERR_MSG: db "Incorrect num of sectors read", 0
