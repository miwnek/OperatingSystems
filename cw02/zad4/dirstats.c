#include <sys/stat.h>
#include <ftw.h>
#include <stdio.h>

long long summed_size = 0;

int pass_to_ftw(const char *name, const struct stat *ptr, int flag);

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("Wrong number of arguments\n");
    return 1;
  }

  int ftw_result = ftw(argv[1], pass_to_ftw, 1);

  if(ftw_result != 0) {
    printf("Coudln't read files in specified directory and its subdirectories.\n");
    return 1;
  }

  printf("Summed size of all files in bytes: %lld.\n", summed_size);

  return 0;
}

int pass_to_ftw(const char *name, const struct stat *ptr, int flag) {
  if(flag == FTW_D) return 0;

  struct stat temp;
  stat(name, &temp);
  printf("File \"%s\" has a size of %ld bytes.\n", name, temp.st_size);
  summed_size += temp.st_size;

  return 0;
}
