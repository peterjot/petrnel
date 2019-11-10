#include "screen.h"
#include "ports.h"

// PRIVATE FUNCTIONS
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

// PUBLIC FUNCTIONS

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void print_at(char *message, int col, int row) {
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void print(char *message) {
    print_at(message, -1, -1);
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    
    char *video_mem = (char*)VIDEO_ADDRESS;

    int i;
    for (i = 0; i < screen_size; i++) {
        video_mem[i*2] = ' ';
        video_mem[i*2+1] = WHITE_ON_BLACK;
    }

    set_cursor_offset(get_offset(0, 0));
}

int print_char(char character, int col, int row, char attribute) {
    unsigned char *video_mem = (unsigned char*) VIDEO_ADDRESS;
    
    if (!attribute){
        attribute = WHITE_ON_BLACK;
    }

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        video_mem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        video_mem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (character == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        video_mem[offset] = character;
        video_mem[offset+1] = attribute;
        offset += 2;
    }
    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {
    /* Use the VGA ports to get the current cursor position
     * 14 - high byte of the cursor offset
     * 15 - low byte
     */
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col); 
}

int get_offset_row(int offset) { 
    return offset / (2 * MAX_COLS); 
}

int get_offset_col(int offset) { 
    return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; 
}
