#ifndef __LIBTYPESET__
#define __LIBTYPESET__

#pragma once

typedef struct {
    int lines;
    int words;
    int chars;
} Wc_result;

typedef struct {
    Wc_result **pointer_array;
    int max_array_size;
    int current_array_size;
} Extended_array;

extern Extended_array (*initialize_extended_array)();
extern int (*execute_wc)(Extended_array *exar, char *filename);
extern Wc_result* (*get_block_on_index)(Extended_array *exar, int index);
extern void (*free_block_on_index)(Extended_array *exar, int index);
extern void (*free_array)(Extended_array* exar);

#endif

//snprintf
