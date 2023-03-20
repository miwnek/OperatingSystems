#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("Wrong number of arguments.\n");
    return 1;
  }

  int num = atoi(argv[1]);
  pid_t process_id = -1;

  for(int i = 0; i < num; i++) {
    process_id = fork();

    if(process_id == 0) {
      printf("Current process id: %d, parent process id: %d.\n", getpid(), getppid());
      exit(0);
    }
    else {
      wait(NULL);
    }
  }

  printf("argv[1]: %d\n", num);

  return 0;
}
