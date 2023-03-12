#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 1000
#define BUFFER_SIZE 1024

Extended_array initialize_extended_array() {
    Extended_array product;
    product.max_array_size = MAX_SIZE;
    product.current_array_size = 0;
    product.pointer_array = calloc(MAX_SIZE, sizeof(Wc_result*));
    return product;
}

int execute_wc(Extended_array* exar, char* filename) {
    
    char temp_filename[] = "/tmp/temp_XXXXXX";
    int temp_file_no = mkstemp(temp_filename);

    if (temp_file_no == 0) {
        printf("Error: Couldn't create temporary file.");
        return 0;
    }
    // Performing wc and putting its result into a temp file
    char command[BUFFER_SIZE] = "";
    snprintf(command, BUFFER_SIZE, "wc '%s' > '%s'", filename, temp_filename);
    system(command);

    FILE *temp_file = fopen(temp_filename, "r");

    if(temp_file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return 0;
    }

    char* buffer = malloc(BUFFER_SIZE * sizeof(char));
    fgets(buffer, BUFFER_SIZE, temp_file);

    int l, w, c;
    if (!(3 == sscanf(buffer,
                    "%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d",
                    &l, &w, &c))) {
                        printf("Error: couldn't parse buffer.");
                        return 0;
                    };

    Wc_result *res = malloc(sizeof *res);
    exar->pointer_array[exar->current_array_size] = res;
    exar->current_array_size++;

    res->lines = l;
    res->words = w;
    res->chars = c;

    fclose(temp_file);
    free(buffer);

    char rm_command[20] = "rm ";
    strcat(rm_command, temp_filename);
    system(rm_command);

    return 1;
}

Wc_result* get_block_on_index(Extended_array* exar, int index) {
    if(index > exar->current_array_size - 1 || index < 0) {
        printf("Incorrect index.\n");
        Wc_result *failed = malloc(sizeof *failed);
        failed->lines = -1;
        failed->words = -1;
        failed->chars = -1;
        return failed;
    }
    return exar->pointer_array[index];
}

void free_block_on_index(Extended_array* exar, int index) {
    if(index > exar->current_array_size - 1 || index < 0) {
        return;
    }
    free(exar->pointer_array[index]);
}

void free_array(Extended_array* exar) {
    for(int i = 0; i < exar->current_array_size; i++) {
        free(exar->pointer_array[i]);
    }
    free(exar->pointer_array);
    exar->current_array_size = -1;
}
