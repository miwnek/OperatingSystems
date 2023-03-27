#include <bits/types/siginfo_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

typedef enum  {
  PN,
  PT,
  PRN,
  PTL,
  TT
} USER_ACTION;

int requests_made = 0;
USER_ACTION current_mode = PN;
int take_action = 0;
int sender_process_id = -1;

void print_number() {
  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 10; j++) {
      printf("%d ", 10 * i + j);
    }
    printf("\n");
  }
}

void print_time() {
  time_t rawtime;
  struct tm *timeinfo;
  time (&rawtime);
  timeinfo = localtime(&rawtime);
  printf("Current time and date: %s.\n", asctime(timeinfo));
}

void print_request_num() {
  printf("Number of requests made: %d\n.", requests_made);
}

void print_time_loop() {
  setbuf(stdout, NULL);
  while(current_mode == PTL) {
    print_time();
    sleep(1);
  }
}

void handler(int signum, siginfo_t *siginfo, void *context){
  requests_made += 1;
  sender_process_id = siginfo->si_pid;
  int action = siginfo->si_status;
  if(action == 1) {
    current_mode = PN;
  } else if (action == 2) {
    current_mode = PT;
  } else if (action == 3) {
    current_mode = PRN;
  } else if (action == 4) {
    current_mode = PTL;
  } else {
    current_mode = TT;
  }
  take_action = 1;
  kill(sender_process_id, SIGUSR1);
}


int main() {
  printf("Catcher's process id: %d.\n", getpid());

  struct sigaction sa;
  sa.sa_sigaction = &handler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &sa, NULL);

  while(1) {
    if(take_action == 1) {
      switch (current_mode) {
      case PN:
        print_number();
        break;
      case PT:
        print_time();
        break;
      case PRN:
        print_request_num();
        break;
      case PTL:
        print_time_loop();
        break;
      default:
        exit(0);
        break;
      }
      take_action = 0;
    }
  }
  exit(1);
}
