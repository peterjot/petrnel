#include "../drivers/screen.h"
#include "util.h"

void main() {
  clear_screen();

  int i;
  for(i = 0; i< 24; i++){
    char str[10];
    int_to_ascii(i, str);
    print(str);
    print(" ");
  }

//  print_at("What happens when we run out of space?", 70, 24);
}
