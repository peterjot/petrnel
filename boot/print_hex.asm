; receiving the data in 'dx' register
print_hex:
    PUSHA

    MOV CX, 0



hex_loop:
    CMP CX, 4
    JE end

    MOV AX, DX
    AND AX, 0x000F; mask three digits to zero
    ADD AL, 0x30 
    CMP AL, 0x39
    JLE step2
    ADD AL, 7

step2:
    MOV BX, HEX_OUT + 5
    SUB BX, CX
    MOV [BX], AL
    ROR DX, 4

    ADD CX, 1
    JMP hex_loop

end:
    MOV BX, HEX_OUT
    CALL print

    POPA
    RET

HEX_OUT:
    db '0x0000', 0
