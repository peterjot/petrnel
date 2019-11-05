print:
    PUSHA


start:
    MOV AL, [BX]
    CMP AL, 0
    JE done


    MOV AH, 0x0E
    INT 0x10

    ADD BX, 1
    JMP start

done:
    POPA
    RET

print_nl:
    PUSHA

    MOV AH, 0x0E
    MOV AL, 0x0A
    INT 0x10
    MOV AL, 0x0D
    INT 0x10
    
    POPA
    RET
