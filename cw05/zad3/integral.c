#include <bits/time.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define QUEUE_PATH "/tmp/_queue_"

long double rectangle_width = 0.0;

void fill_todo_list(int process_count, int *todo_list, int *start_list);

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Wrong number of arguments");
        exit(1);
    }
    setbuf(stdout, NULL);

    char *string_waste;

    rectangle_width = strtold(argv[1], &string_waste);
    int process_count = atoi(argv[2]);

    if (rectangle_width > 1 || rectangle_width <= 0) {
       fprintf(stderr,"Rectangle width has to be between 0 and 1.\n");
       exit(2);
    }
    if(process_count <= 0) {
        fprintf(stderr, "Process count has to be a positive integer.\n");
        exit(3);
    }

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    int *todo_list = calloc(sizeof(int), process_count);
    int *start_list = calloc(sizeof(int), process_count);

    fill_todo_list(process_count, todo_list, start_list);

    mkfifo(QUEUE_PATH, 0666);

    char buff1[20], buff2[20], buff3[5000];
    for(int i = 0; i < process_count; i++) {
        if(fork() != 0) {
            snprintf(buff1, 20, "%d", todo_list[i]);
            snprintf(buff2, 20, "%d", start_list[i]);
            snprintf(buff3, 5000, "%0.19Lf", rectangle_width);
            execl("./subprocess", "subprocess", buff3, buff1, buff2, NULL);
        }
        else wait(0);
    }

    long double sum = 0.0;
    char buffer[20000] = "";

    FILE *stream = fopen(QUEUE_PATH, "r");
    fread(buffer, sizeof(char), 20000, stream);

    char sep_char[1] = "$";
    char * temp;
    temp = strtok( buffer, sep_char );
    while( temp != NULL )
    {
        sum += strtold(temp, &string_waste);
        temp = strtok( NULL, sep_char );
    }

    fclose(stream);
    remove(QUEUE_PATH);

    clock_gettime(CLOCK_REALTIME, &end);

    long difference;
    char unit[3] = "";
    if(end.tv_sec - start.tv_sec > 0) {difference = end.tv_sec - start.tv_sec; strcpy(unit, "s");}
    else {difference = end.tv_nsec - start.tv_nsec; strcpy(unit, "ns");}
    char buff[200];
    snprintf(buff, 200,
             "Integral: %Lf\nProcess count: %d\nRectangle width: %Lf.\nTook: %ld %s.\n\n",
             sum, process_count, rectangle_width, difference, unit);
    int to_write = strlen(buff);
    FILE* fd = fopen("results.txt", "a");
    fwrite(buff, sizeof(char), to_write, fd);
    fclose(fd);

    printf("%s", buff);

    return 0;
}

void fill_todo_list(int process_count, int *todo_list, int *start_list) {
   int rectangle_count = ceil(1. / rectangle_width);
   int for_all_processes = rectangle_count / process_count;
   int remainding = rectangle_count % process_count;

   for(int i = 0; i < process_count; i++) {
       todo_list[i] = for_all_processes;
   }
   if(remainding != 0) {
       int idx = 0;
       while(remainding > 0) {
           todo_list[idx] += 1;
           remainding--;
           idx++;
       }
   }

   int sofar = 0;
   for(int i = 0; i < process_count; i++) {
       start_list[i] = sofar;
       sofar += todo_list[i];
   }
}
