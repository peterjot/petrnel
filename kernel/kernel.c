#include "../drivers/screen.h"
#define SCREEN_CTRL_PORT 0x3d4
#define SCREEN_DATA_PORT 0x3d5

void main() {
//    port_byte_out(0x3d4, 14);
 //   int position = port_byte_in(0x3d5) << 8;
    
 //   port_byte_out(0x3d4, 15);
  //  position += port_byte_in(0x3d5);

   // int offset_from_vga = position * 2;
    
  //  char *vga = (char*) 0xb8000;
  //  vga[offset_from_vga] = 'X';
   // vga[offset_from_vga+1]= 0x0f;
    clear_screen();
//    char* video_memory = (char*) 0xb8000;
//    print_at("7", 0, 6);
//    *video_memory = 'F';
//    video_memory+=2;
 //   *video_memory = 'B';
 //   video_memory+=2;
 //   *video_memory = 'C';
  //  print_at("This text spans multiple lines", 75, 10);
  //  print_at("There is a line\nbreak", 0, 20);
  //  print("There is a line\nbreak\n");
    print("Smiesznie\ntak");
  //  print_at("What happens when we run out of space?", 70, 24);
}
