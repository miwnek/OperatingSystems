#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  DIR *dd_current = opendir(".");

  if(dd_current == NULL) {
    printf("Couldn't open current directory.");
    return 1;
  }

  long long summed_size = 0;
  struct dirent *examined_file;
  struct stat file_stats;

  while((examined_file = readdir(dd_current)) != NULL) {
    stat(examined_file->d_name, &file_stats);

    if (!S_ISDIR(file_stats.st_mode)) {
      printf("File \"%s\" has a size of %ld bytes.\n", examined_file->d_name, file_stats.st_size);
      summed_size += file_stats.st_size;
    }
  }

  printf("Summed size of all files in bytes: %lld.\n", summed_size);

  closedir(dd_current);

  return 0;
}
