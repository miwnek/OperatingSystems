#include "libfiles.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  if(argc != 5) {
    printf("Wrong number of arguments.\n");
    return 1;
  }

  if(strlen(argv[1]) != 1 || strlen(argv[2]) != 1) {
    printf("First two arguments must be single ASCII characters.\n");
    return 1;
  }

  #ifdef SYS_FUNCS
  EXIT_CODE code = swap_chars_system(argv[1][0], argv[2][0], argv[3], argv[4]);
  #else
  EXIT_CODE code = swap_chars_libraries(argv[1][0], argv[2][0], argv[3], argv[4]);
  #endif

  switch(code) {

  case OPEN_ERR:
    printf("Error: couldn't open specified file.\n");
    break;

  case CREAT_ERR:
    printf("Error: couldn't open/create specifed file.\n");
    break;

  default:
    return 0;
    break;
  }

  return 1;
}
