#include "libreverse.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void reverse_string(char *to_reverse) {
  int length = strlen(to_reverse);
  char temp;
  for(int i = 0; i < length / 2; i++) {
    temp = to_reverse[i];
    to_reverse[i] = to_reverse[length - i - 1];
    to_reverse[length - i - 1] = temp;
  }
}

EXIT_CODE reverse_file(char *source, char *target, int block_length) {
  char *block = malloc(sizeof(char) * (block_length + 1));
  FILE *fd_source = fopen(source, "r");

  if (fd_source == NULL) {
    fclose(fd_source);
    return OPEN_ERR;
  }

  FILE *fd_target = fopen(target, "a");

  if (fd_target == NULL) {
    fclose(fd_source);
    fclose(fd_target);
    return CREAT_ERR;
  }

  fseek(fd_source, 0, SEEK_END);
  long file_size = ftell(fd_source);

  long blocks = file_size / (long) block_length;
  int rest = file_size % (long) block_length;

  int checker = 0;
  int checkers = 0;

  int offset = (blocks > 0) ? block_length : rest;
  fseek(fd_source, -offset, SEEK_CUR);
  for(long i = 0; i < blocks; i++) {
    checker += fread(block, sizeof(char), block_length, fd_source);
    reverse_string(block);
    checkers += fwrite(block, sizeof(char), block_length, fd_target);
    fseek(fd_source, -2 * block_length, SEEK_CUR);
  }

  fseek(fd_source, 0, SEEK_SET);
  if(rest > 0) {
    char *block_rest = malloc(sizeof(char) * rest + 1);
    checker += fread(block_rest, sizeof(char), rest, fd_source);
    reverse_string(block_rest);
    checkers += fwrite(block_rest, sizeof(char), rest, fd_target);
    free(block_rest);
  }

  fclose(fd_source);
  fclose(fd_target);
  free(block);

  return SUCCESS;
}
