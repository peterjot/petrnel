unsigned char port_byte_in (unsigned short port) {
    unsigned char byte;
     /* 
     * '"=a" (result)'; set '=' the C variable '(result)' to the value of register EAX
     * '"d" (port)': map the variable '(port)' into EDX
     * Inputs and outputs are separated by colons
     */
    __asm__("in %%dx, %%al" : "=a" (byte) : "d" (port));
    return byte;
}

void port_byte_out (unsigned short port, unsigned char byte) {
    __asm__("out %%al, %%dx" : : "a" (byte), "d" (port));
}

unsigned short port_word_in (unsigned short port) {
    unsigned short word;
    __asm__("in %%dx, %%ax" : "=a" (word) : "d" (port));
    return word;
}

void port_word_out (unsigned short port, unsigned short word) {
    __asm__("out %%ax, %%dx" : : "a" (word), "d" (port));
}
