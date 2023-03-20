#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("Wrong number of arguments.\n");
    return 1;
  }
  setbuf(stdout, NULL);

  int length = strlen(argv[0]);
  char *name = malloc(sizeof(char) * length - 1);

  for(int i = 2; i < length; i++) {
    name[i - 2] = argv[0][i];
  }
  name[length - 2] = 0;

  printf("%s ", name);

  execl("/bin/ls", "ls", argv[1], NULL);

  return 0;
}
