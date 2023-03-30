#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

char fortune_buffer[2048] = "";
char cowsay_buffer[2048] = "cowsay \"";

int main(int argc, char *argv[]) {
 if(argc == 1) {
  FILE* output = popen("fortune", "w");
  pclose(output);
  wait(NULL);
 }

 else if(argc == 2) {
  strcat(cowsay_buffer, argv[1]);
  strcat(cowsay_buffer, "\"");
  FILE* output = popen(cowsay_buffer, "w");
  pclose(output);
  wait(NULL);
 }

 else {
  printf("Wrong number of arguments.\n");
  exit(1);
 }

 return 0;
}
