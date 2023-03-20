#include "libreverse.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Wrong number of arguments.\n");
    return 1;
  }

  struct timespec start_1, end_1;
  struct timespec start_1024, end_1024;

  clock_gettime(CLOCK_REALTIME, &start_1);
  EXIT_CODE code = reverse_file(argv[1], argv[2], 1024);
  clock_gettime(CLOCK_REALTIME, &end_1);

  switch (code) {
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

  clock_gettime(CLOCK_REALTIME, &start_1024);
  code = reverse_file(argv[1], argv[2], 1);
  clock_gettime(CLOCK_REALTIME, &end_1024);

  switch (code) {
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

  size_t needed = snprintf(
      NULL, 0,
      "Reversing file \"%s\" to file \"%s\".\n1024 character at a time: %ld s %ld "
      "ns.\n1 characters at a time: %ld s %ld ns.\n\n",
      argv[1], argv[2], end_1.tv_sec - start_1.tv_sec,
      end_1.tv_nsec - start_1.tv_nsec, end_1024.tv_sec - start_1024.tv_sec,
      end_1024.tv_nsec - start_1024.tv_nsec);

  char *results = malloc(sizeof(char) *  needed + 1);

  sprintf(
      results,
      "Reversing file \"%s\" to file \"%s\".\n1024 character at a time: %ld s %ld "
      "ns.\n1 characters at a time: %ld s %ld ns.\n\n",
      argv[1], argv[2], end_1.tv_sec - start_1.tv_sec,
      end_1.tv_nsec - start_1.tv_nsec, end_1024.tv_sec - start_1024.tv_sec,
      end_1024.tv_nsec - start_1024.tv_nsec);

  FILE *fd_result = fopen("pomiar_zad_2.txt", "a");
  fwrite(results, sizeof(char), needed, fd_result);
  fclose(fd_result);
  free(results);

  return 0;
}
