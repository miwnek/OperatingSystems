#ifndef __LIBFILES__
#define __LIBFILES__

typedef enum {
  SUCCES,
  OPEN_ERR,
  CREAT_ERR,
  WRITE_ERR
} EXIT_CODE;

EXIT_CODE swap_chars_system(char swap_from, char swap_to, char *filepath_in, char *filepath_out);
EXIT_CODE swap_chars_libraries(char swap_from, char swap_to, char *filepath_in, char *filepath_out);

#endif
