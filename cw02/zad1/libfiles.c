#include "libfiles.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>


EXIT_CODE swap_chars_system(char swap_from, char swap_to, char *filepath_in, char *filepath_out) {
  int fd_source = open(filepath_in, O_RDONLY);

  if (fd_source == -1) {
    close(fd_source);
    return OPEN_ERR;
  }

  int fd_target = open(filepath_out, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

  if(fd_target == -1) {
    close(fd_source);
    close(fd_target);
    return CREAT_ERR;
  }

  char c;
  while(read(fd_source, &c, 1) == 1) {
    if(c == swap_from) {
      write(fd_target, &swap_to, 1);
    }
    else {
      write(fd_target, &c, 1);
    }
  }

  close(fd_source);
  close(fd_target);

  return SUCCES;
}


EXIT_CODE swap_chars_libraries(char swap_from, char swap_to, char *filepath_in, char *filepath_out) {
  FILE *fd_source = fopen(filepath_in, "r");

  if (fd_source == NULL) {
    fclose(fd_source);
    return OPEN_ERR;
  }

  FILE *fd_target = fopen(filepath_out, "a");

  char c;
  while (fread(&c, sizeof(char), 1, fd_source) == 1) {
    if (c == swap_from) {
      fwrite(&swap_to, sizeof(char), 1, fd_target);
    } else {
      fwrite(&c, sizeof(char), 1, fd_target);
    }
  }

  fclose(fd_source);
  fclose(fd_target);

  return SUCCES;
}
