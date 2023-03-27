#include <bits/types/siginfo_t.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


void handler(int signum) {
  printf("Sender received feedback from sent signal.\n");
}

int main(int argc, char *argv[]) {
  if(argc < 3) {
    printf("Not enough arguments.\n");
    exit(1);
  }


  int catcher_process_id = atoi(argv[1]);
  int action = 5;

  for(int i = 0; i < argc - 2; i++) {
    action = atoi(argv[i + 2]);
    if(action < 1 || action > 5) {
      printf("Bad argument, won't execute.\n");
      continue;
    }

    struct sigaction sa;
    sa.sa_handler = &handler;
    sigaction(SIGUSR1, &sa, NULL);

    sigval_t signal_value = {action};
    sigqueue(catcher_process_id, SIGUSR1, signal_value);
    printf("Sender sent signal with value %d.\n", action);
  }

  exit(0);
}
