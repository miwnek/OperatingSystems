#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void sig_handler() { printf("Received SIGUSR1 and handled it.\n"); }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments.\n");
    exit(1);
  }

  if (strcmp(argv[1], "ignore") == 0) {
    signal(SIGUSR1, SIG_IGN);
  } else if (strcmp(argv[1], "handler") == 0) {
    signal(SIGUSR1, sig_handler);
  } else if (strcmp(argv[1], "mask") == 0) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
  } else if (strcmp(argv[1], "pending") == 0) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
  } else {
    fprintf(stderr,
            "Invalid argument: \"%s\", try: ignore|handler|mask|pending.",
            argv[1]);
    exit(1);
  }

  setbuf(stdout, NULL);
  printf(" -- parent process -- \n");
  raise(SIGUSR1);
  if (strcmp(argv[1], "pending") == 0) {
    sigset_t set;
    sigpending(&set);
    if (sigismember(&set, SIGUSR1)) {
      printf("SIGUSR1 is pending in parent process.\n");
    } else {
      printf("SIGUSR1 is not pending in parent process.\n");
    }
  }

  pid_t process_id = fork();
  if(process_id == -1) {
    perror("fork");
    exit(1);
  } else if(process_id == 0) {
    printf(" -- child process -- \n");
    if(strcmp(argv[1], "pending") == 0) {
      sigset_t set;
      sigpending(&set);
      if(sigismember(&set, SIGUSR1)) {
        printf("SIGUSR1 is pending in child process.\n");
      } else {
        printf("SIGUSR1 is not pending in child process.\n");
      }
    } else {
      raise(SIGUSR1);
    }
    exit(0);
  } else {
    wait(NULL);
  }

  return 0;
}
