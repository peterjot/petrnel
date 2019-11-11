#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"


// PRIVATE FUNCTIONS
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char character, int col, int row, char attr);
char* last_row_address();
int handle_scroll(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

// PUBLIC FUNCTIONS


// get cursor offset when col or row is negative
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
  const int screen_size = (MAX_COLS * MAX_ROWS);
  
  char* const video_mem = (char*)VIDEO_ADDRESS;

  int i;
  for (i = 0; i < screen_size; i++) {
      video_mem[i*2] = ' ';
      video_mem[i*2+1] = WHITE_ON_BLACK;
  }

  set_cursor_offset(get_offset(0,0));
}

void set_error_flag(){
  char* const video_mem = (char*) VIDEO_ADDRESS;
  video_mem[2 * (MAX_COLS) * (MAX_ROWS) - 2] = 'E';
  video_mem[2 * (MAX_COLS) * (MAX_ROWS) - 1] = RED_ON_WHITE;
}

int print_char(const char character, const int col, int row, char attribute) {
  char* const video_mem = (char*) VIDEO_ADDRESS;
  
  if (col >= MAX_COLS || row >= MAX_ROWS) {
    set_error_flag();
    return get_offset(col, row);
  }

  if (!attribute) {
    attribute = WHITE_ON_BLACK;
  }

  int offset;
  if (col >= 0 && row >= 0) {
    offset = get_offset(col, row);
  } else {
    offset = get_cursor_offset();
  }

  if (character == '\n') {
    row = get_offset_row(offset);
    offset = get_offset(0, row + 1);
  } else {
    video_mem[offset] = character;
    video_mem[offset + 1] = attribute;
    offset += 2;
  }

  offset = handle_scroll(offset);
  set_cursor_offset(offset);
  return offset;
}

int handle_scroll(int offset){
  const int max_screen_offset = MAX_ROWS * MAX_COLS * 2;
  if (offset >= max_screen_offset) {
    const int row_size = MAX_COLS * 2;

    int i;
    for (i = 1; i < MAX_ROWS; i++){ 
      char* const line_address_source = (char*) (get_offset(0, i) + VIDEO_ADDRESS);
      char* const line_above_address_dest = (char*) (get_offset(0, i-1) + VIDEO_ADDRESS);
      memory_copy(
        line_address_source, 
        line_above_address_dest,
        row_size
      );
    }

    char* const last_line = last_row_address();
    for (i = 0; i < row_size; i++){
      last_line[i] = 0;
    }
    offset -= row_size;
  }
  return offset;
}

char* last_row_address() {
  return (char*) get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
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

int get_offset(const int col, const int row) {
  return 2 * (row * MAX_COLS + col); 
}

int get_offset_row(const int offset) { 
  return offset / (2 * MAX_COLS); 
}

int get_offset_col(const int offset) { 
  return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; 
}
