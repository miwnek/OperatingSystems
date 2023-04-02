#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_PATH "/tmp/_queue_"

long double function(long double x) {
    return 4./(x*x + 1.);
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        fprintf(stderr, "Wrong number of arguments given to subprocess.\n");
        exit(1);
    }
    char *string_waste;

    long double rectangle_width = strtold(argv[1], &string_waste);
    int todo = atoi(argv[2]);
    int start = atoi(argv[3]);

    long double partial_sum = 0.0;
    for(int i = 0; i < todo; i++) {
        partial_sum += rectangle_width * function( ((long double)(start + i)) * rectangle_width );
    }

    char buffer[20] = "";
    int to_write = snprintf(buffer, 20, "%Lf$", partial_sum);

    FILE *stream = fopen(QUEUE_PATH, "a");
    setbuf(stream, NULL);
    if(fwrite(buffer, sizeof(char), to_write, stream) == -1) {
        perror("");
    }
    fclose(stream);
    return 0;
}
