#ifndef __LIB_DEF__
#define __LIB_DEF__

typedef enum {
  SUCCESS,
  OPEN_ERR,
  CREAT_ERR,
  WRITE_ERR
} EXIT_CODE;


void reverse_string(char *to_reverse);
EXIT_CODE reverse_file(char *source, char *target, int block_length);

#endif
