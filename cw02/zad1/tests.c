#include "libfiles.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

  if(argc != 5) {
    printf("Wrong number of arguments.\n");
    return 1;
  }

  if(strlen(argv[1]) != 1 || strlen(argv[2]) != 1) {
    printf("First two arguments must be single ASCII characters.\n");
    return 1;
  }

  struct timespec sys_start, sys_end;
  struct timespec lib_start, lib_end;

  clock_gettime(CLOCK_REALTIME, &sys_start);
  EXIT_CODE code = swap_chars_system(argv[1][0], argv[2][0], argv[3], argv[4]);
  clock_gettime(CLOCK_REALTIME, &sys_end);

  switch (code) {
  case OPEN_ERR:
    printf("Error: couldn't open specified file.\n");
    return 1;
    break;

  case CREAT_ERR:
    printf("Error: couldn't open/create specifed file.\n");
    return 1;
    break;

  case WRITE_ERR:
    printf("Error: couldn't write to specified file.\n");
    return 1;
    break;

  default:
    break;
  }

  clock_gettime(CLOCK_REALTIME, &lib_start);
  code = swap_chars_libraries(argv[1][0], argv[2][0], argv[3], argv[4]);
  clock_gettime(CLOCK_REALTIME, &lib_end);

  switch (code) {
  case OPEN_ERR:
    printf("Error: couldn't open specified file.\n");
    return 1;
    break;

  case CREAT_ERR:
    printf("Error: couldn't open/create specifed file.\n");
    return 1;
    break;

  case WRITE_ERR:
    printf("Error: couldn't write to specified file.\n");
    return 1;
    break;

  default:
    break;
  }

  size_t needed = snprintf(
      NULL, 0,
      "Replacing \"%c\" from %s with \"%c\" to %s.\nSystem functions: %ld "
      "ns.\nLibrary functions: %ld ns.\n\n",
      argv[1][0], argv[3], argv[2][0], argv[4],
      sys_end.tv_nsec - sys_start.tv_nsec, lib_end.tv_nsec - lib_start.tv_nsec);

  char *buffer = malloc(needed);
  sprintf(
      buffer,
      "Replacing \"%c\" from %s with \"%c\" to %s.\nSystem functions: %ld "
      "ns.\nLibrary functions: %ld ns.\n\n",
      argv[1][0], argv[3], argv[2][0], argv[4],
      sys_end.tv_nsec - sys_start.tv_nsec, lib_end.tv_nsec - lib_start.tv_nsec);

  FILE *fd_result = fopen("pomiar_zad_1.txt", "a");
  fwrite(buffer, sizeof(char), needed, fd_result);
  fclose(fd_result);
  free(buffer);

  return 0;
}
