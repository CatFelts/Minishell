/*
 * CS 352 -- Builtin Commands for Minishell
 * Cat Felts
 * Oct 1, 2016
 * Assignment 2
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "proto.h"



/* Constants */
#define CMDLISTLEN 2

/* Global Variables */
const char *cmd_list[] = {
  "exit",
  "aecho"
};


/* Helper Functions */
void exit_cmd(char **argv, int argc)
{
  if(strcmp(argv[0], "exit")!= 0){
    fprintf(stderr, "Error: Invalid exit call\n");
    return;
  }

  char *exit_arg;
  int exit_val = 0;
  
  if(argc>1){
    exit_arg = argv[1];
    exit_val = atoi(exit_arg);
  }
  
  exit(exit_val);
  fprintf(stderr, "Error: exit failure\n");
}


void aecho_cmd(char **argv, int argc){
  if(strcmp(argv[0], "aecho") != 0){
    fprintf(stderr, "Error: Invalid call to aecho command.\n");
    return;
  }

  int arg_start = 1;
  int fd = 1;
  
  if(argc ==1){
    dprintf(fd, "\n");
    return;
  }
  
  if(strcmp(argv[1], "-n") == 0)
    arg_start = 2;

  for(int i =arg_start; i<argc-1; i++){
    dprintf(fd, "%s ", argv[i]);
  }
  if(argc>2 || arg_start == 1)
    dprintf(fd, "%s", argv[argc-1]);
  if(arg_start == 1)
    dprintf(fd, "\n");
  
  
  return;
}


/* Check if argument is a builtin command */
int check_cmd(char **argv, int argc){
  int return_val = -1;
  
  for(int i = 0; i<CMDLISTLEN; i++){
    if(strcmp(argv[0], cmd_list[i]) == 0){
      return_val = i;
    }
  }

  return return_val;
}


/* Execute Command */
int execute_cmd(char **argv, int argc, int cmd_val){
  switch(cmd_val){

  case 0:         //"exit" command
    exit_cmd(argv, argc);
    break;
    
  case 1:        //"aecho" command
    aecho_cmd(argv, argc);
    break;
    
  default:
    fprintf(stderr, "Error: invalid command");
  }
  
  return 0;
}
