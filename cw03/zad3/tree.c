#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

#define MAX_BUFFER_SIZE 255

char read_sequence[MAX_BUFFER_SIZE + 1];
DIR *curr_dir;
struct dirent *dir_entry;
struct stat content_stats;
struct stat entry_stats;

int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("Wrong number of arguments.\n");
    return 1;
  }

  char *this_path = realpath(argv[0], NULL);

  if(this_path == NULL ) {
    perror(argv[0]);
    return 1;
  }

  char *target_path = realpath(argv[1], NULL);

  if(target_path == NULL) {
    perror(argv[1]);
    return 1;
  }

  if(stat(target_path, &content_stats) == -1){
    perror(target_path);
    return 1;
  }

  if(S_ISDIR(content_stats.st_mode)) {
    curr_dir = opendir(target_path);

    if(curr_dir == NULL) {
      perror(target_path);
      return 1;
    }

    char entry_path[PATH_MAX] = "";
    while((dir_entry = readdir(curr_dir)) != NULL) {
      strcpy(entry_path, target_path);
      strcat(entry_path, "/");
      strcat(entry_path, dir_entry->d_name);

      if (stat(entry_path, &entry_stats) == -1) {
        closedir(curr_dir);
        perror(entry_path);
        return 1;
      }

      if ((dir_entry->d_name[0] == '.') && S_ISDIR(entry_stats.st_mode)) {
        continue;
      }

      int process_id = fork();
      if (process_id == 0) {
        execl(this_path, this_path, entry_path, argv[2], NULL);
      } else {
        wait(NULL);
      }
    }

  }
  else {
    FILE *fd = fopen(target_path, "r");
    if (fd == NULL) {
      perror(target_path);
      return 1;
    }
    int seq_length = strlen(argv[2]);
    int read_length = fread(read_sequence, sizeof(char), seq_length, fd);
    if(ferror(fd)) {
      fclose(fd);
      perror(NULL);
      return 1;
    }
    if((read_length == seq_length) && (strcmp(argv[2], read_sequence) == 0)) {
      setbuf(stdout, NULL);
      printf("File path: %s, process id: %d.\n", target_path, getpid());
    }
    fclose(fd);
  }


  return 0;
}
