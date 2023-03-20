#include "libreverse.h"

#include <stdio.h>

#ifdef BLOCK_LENGTH

int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("Wrong number of arguments.\n");
    return 1;
  }

  EXIT_CODE code = reverse_file(argv[1], argv[2], BLOCK_LENGTH);

  switch(code) {
  case OPEN_ERR:
    printf("Couldnt't open first specified file.\n");
    return 1;
    break;
  case CREAT_ERR:
    printf("Couldn't open/create second specified file.\n");
    return 1;
    break;
  default:
    break;
  }

  return 0;
}
#else

int main(int argc, char** argv) {
  printf("Buffer size was not specified during compilation.\n");
  return 1;
}

#endif
