#ifdef LIBTYPE_DLL
    #include "library_dll.h"
#else
    #include "library.h"
#endif

#include "dllhandle.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<sys/times.h>
#include<string.h>

#define BUFFER_SIZE 1024

typedef enum { INIT, COUNT, SHOW, DELETE, DESTROY, WRONG, QUIT } ACTION;

const char *init_pattern = "init [0-9][0-9]*";
const char *count_pattern = "count ..*";
const char *show_pattern = "show [0-9][0-9]*";
const char *delete_pattern = "delete [0-9][0-9]*";
const char *destroy_pattern = "destroy\n";
const char *quit_pattern = "quit\n";

int extended_array_exists = 0; // 0 - doesn't exist, 1 - exists

ACTION parse_action(char *action_str) {
  regex_t regex;
  int check = regcomp(&regex, init_pattern, 0);

  if (regexec(&regex, action_str, (size_t)0, NULL, 0) == 0) {
    return INIT;
  }
  check = regcomp(&regex, count_pattern, 0);

  if (regexec(&regex, action_str, (size_t)0, NULL, 0) == 0) {
    return COUNT;
  }
  check = regcomp(&regex, show_pattern, 0);

  if (regexec(&regex, action_str, (size_t)0, NULL, 0) == 0) {
    return SHOW;
  }
  check = regcomp(&regex, delete_pattern, 0);

  if (regexec(&regex, action_str, (size_t)0, NULL, 0) == 0) {
    return DELETE;
  }
  check = regcomp(&regex, destroy_pattern, 0);

  if (regexec(&regex, action_str, (size_t)0, NULL, 0) == 0) {
    return DESTROY;
  }
  check = regcomp(&regex, quit_pattern, 0);

  if (regexec(&regex, action_str, (size_t)0, NULL, 0) == 0) {
    return QUIT;
  }

  if(check){return WRONG;}
  return WRONG;
}

void perform_action(ACTION action, char *action_str, Extended_array* exar) {
  load_dlls("library.so");
  int arg;
  char filename[BUFFER_SIZE];
  Wc_result* target_block = malloc(sizeof(Wc_result*));

  switch(action) {
  case INIT:
    sscanf(action_str, "init %d", &arg);
    if(arg < 1) {
      printf("Size has to be a positive integer.\n");
      return;
    }
    if(extended_array_exists == 0) {
      *exar = initialize_extended_array(arg);
      extended_array_exists = 1;
    } else {
      printf("'init' has already been already run, 'destroy' before running it again.\n");
    }
    break;
  case COUNT:
    if(exar->current_array_size >= exar->max_array_size) {
      printf("Initialised space is full.\n");
      return;
    }
    sscanf(action_str, "count %s\n", filename);
    arg = execute_wc(exar, filename);
    if(arg == 0) {
      printf("\nCouldn't perform wc on a specified file.\n");
    }
    break;
  case SHOW:
    sscanf(action_str, "show %d", &arg);
    if(arg < 0) {
      printf("Enter a nonnegative index.\n");
      return;
    }
    if(arg > exar->current_array_size - 1) {
      printf("Index too high.\n");
      return;
    }
    target_block = get_block_on_index(exar, arg);
    if(target_block->lines == 6 && target_block->chars == 0){
      printf("Can't read into a previously deleted block.\n");
      return;
    }
    printf("lines: %d, words: %d, characters: %d, path: %s\n", target_block->lines, target_block->words, target_block->chars, target_block->filepath);
    break;
  case DELETE:
    sscanf(action_str, "delete %d", &arg);
    free_block_on_index(exar, arg);
    break;
  case DESTROY:
    if(extended_array_exists == 0) {
      printf("Nothing to destroy.\n");
      return;
    }
    free_array(exar);
    extended_array_exists = 0;
    break;
  case QUIT:
    break;
  default:
    printf("Unkown command.\n");
    break;
  }
}


int main() {
  char buffer[BUFFER_SIZE];
  ACTION curr_action = WRONG;
  Extended_array *global_extended_array = malloc(sizeof(Extended_array *));
  struct timespec real_start, real_end;
  struct tms usr_sys_start, usr_sys_end;

  //Starting screen
  printf("REPL interface started, available options: init number, count filename,\nshow number, delete number, destroy, quit.\nrepl> ");
  fgets(buffer, BUFFER_SIZE, stdin);

  clock_gettime(CLOCK_REALTIME, &real_start);
  times(&usr_sys_start);

  curr_action = parse_action(buffer);

  while(curr_action != QUIT) {
    perform_action(curr_action, buffer, global_extended_array);

    clock_gettime(CLOCK_REALTIME, &real_end);
    times(&usr_sys_end);

    printf("Real time: %ld ns.\n", real_end.tv_nsec - real_start.tv_nsec);
    // Should work, but the library function is not implemented correctly??
    printf("User time: %ld ticks.\n", usr_sys_end.tms_cutime - usr_sys_start.tms_cutime);
    printf("Real time: %ld ticks.\n", usr_sys_end.tms_cstime - usr_sys_start.tms_cstime);
    printf("repl> ");
    fgets(buffer, 100, stdin);

    clock_gettime(CLOCK_REALTIME, &real_start);
    times(&usr_sys_start);

    curr_action = parse_action(buffer);
  }

  return 0;
  }
