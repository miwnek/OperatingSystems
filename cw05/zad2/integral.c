#include <bits/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

long double rectangle_width = 0;

long double function(long double x) {
    return 4. / (x*x + 1.);
}

long double rectangle_area(int todo, int start_idx) {
    long double partial_sum = 0;
    long double x_start;
    for(int i = 0; i < todo; i++) {
        x_start = rectangle_width * (start_idx + i);
        partial_sum += rectangle_width * function(x_start);
    }
    return partial_sum;
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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: /.integral \"rect_width\" \"process_num\"\n");
        exit(1);
    }
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

    int *processes_todo = calloc(sizeof(int),  process_count);
    int *processes_start_idx = calloc(sizeof(int),  process_count);

    fill_todo_list(process_count, processes_todo, processes_start_idx);

    pid_t process_id;
    int *fds_read = malloc(process_count * sizeof(int));
    for(int i = 0; i < process_count; i++) {
        if(processes_todo[i] == 0) break;
        int fd_temp[2];
        if(pipe(fd_temp) == -1) perror("");
        process_id = fork();
        if(process_id == 0) {
            close(fd_temp[0]);
            long double partial_area = rectangle_area(processes_todo[i], processes_start_idx[i]);
            if (write(fd_temp[1], &partial_area, sizeof(long double)) == -1) {
                perror("");
                exit(6);
            }
            exit(0);
        }
        else {
            close(fd_temp[1]);
            fds_read[i] = fd_temp[0];
        }
    }
    long double sum = 0.0;
    long double partial_sum;
    for(int i = 0; i <  process_count; i++) {
        if(processes_todo[i] == 0) break;
        read(fds_read[i], &partial_sum, sizeof(long double));
        sum += partial_sum;
        close(fds_read[i]);
    }
    while(wait(NULL) > 0);

    clock_gettime(CLOCK_REALTIME, &end);

    long difference;
    char unit[4] = "";
    if(end.tv_sec - start.tv_sec > 0) {difference = end.tv_sec - start.tv_sec; strcpy(unit, "s.");}
    else {difference = end.tv_nsec - start.tv_nsec; strcpy(unit, "ns.");}
    char buffer[200];
    snprintf(buffer, 200,
             "Integral: %Lf\nProcess count: %d\nRectangle width: %Lf.\nTook: %ld %s.\n\n",
             sum, process_count, rectangle_width, difference, unit);
    int to_write = strlen(buffer);
    FILE* fd = fopen("results.txt", "a");
    fwrite(buffer, sizeof(char), to_write, fd);
    fclose(fd);

    printf("%s", buffer);

    free(fds_read);
    free(processes_todo);
    free(processes_start_idx);
    return 0;
}
