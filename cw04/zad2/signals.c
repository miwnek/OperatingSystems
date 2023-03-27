#include <bits/types/siginfo_t.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void siginfo_handler(int signum, siginfo_t *siginfo, void *context) {
  printf("Received signal %d from process %d,\n", siginfo->si_signo, siginfo->si_pid);
  printf("User data: %d.\n", siginfo->si_value.sival_int);
}

int main() {
  struct sigaction sa;
  sa.sa_sigaction = &siginfo_handler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &sa, NULL);
  kill(getpid(), SIGUSR1);

  sa.sa_flags = SA_ONSTACK | SA_SIGINFO;
  sigaction(SIGUSR1, &sa, NULL);

  kill(getpid(), SIGUSR1);

  sa.sa_flags = SA_RESETHAND;
  sigaction(SIGUSR1, &sa, NULL);

  kill(getpid(), SIGUSR1);
  kill(getpid(), SIGUSR1);
  return 0;
}
