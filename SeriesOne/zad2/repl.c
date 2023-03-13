#include "library.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

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
      printf("Couldn't perform wc on a specified file.\n");
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
      printf("Can't read into a previously destroyed block.\n");
      return;
    }
    printf("lines: %d, words: %d, characters: %d\n", target_block->lines, target_block->words, target_block->chars);
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

  //Starting screen
  printf("REPL interface started, available options: init number, count filename,\nshow number, delete number, destroy, quit.\nrepl> ");
  fgets(buffer, BUFFER_SIZE, stdin);
  curr_action = parse_action(buffer);

  while(curr_action != QUIT) {
    perform_action(curr_action, buffer, global_extended_array);
    printf("repl> ");
    fgets(buffer, 30, stdin);
    curr_action = parse_action(buffer);
  }

  return 0;
  }
