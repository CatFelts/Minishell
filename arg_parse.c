/*
 * CS 352 -- Parse Arguments for Minishell
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

/* Prototypes */
void slide(char *line, int src, int dest);

char **arg_parse(char *line, int *argcp)
{
  int index = 0;
  int num_args = 0;
  int temp_start_index = 0;
  int result_index = 0;
  bool args_counted = false;
  bool args_done = false;
  bool in_quotes = false;
  
  /* first parse: count arguments */
  while(!args_counted){
    
    /* skip leading whitespace */
    while(line[index] == ' ')
      index++;

    if(line[index]!= '\0')
      num_args++;
    /* walk thru word */
    while((line[index] != ' ' || in_quotes) && line[index] != '\0'){
      if(line[index] == '"')
	in_quotes = !in_quotes;
      index++;
    }
    if(line[index]=='\0'){
      args_counted = true;
      if(in_quotes){
	fprintf(stderr, "Error: odd number of quotes\n");
	return NULL;
      }
    }
  }
  

  /* second parse: assign pointers and insert null terminators */
  index = 0;

  char **result = (char **)malloc((sizeof(char *))*(num_args+1));
  if(result == NULL){
    fprintf(stderr , "Error: memory allocation failed.\n");
    return NULL;                                                      
  }
  
  while(!args_done){

    /* skip leading whitespace */
    while(line[index] == ' ')
      index++;

    /* assign pointer to start of word, remove quotes, and attach null terminator to end of word */
    temp_start_index = index;

    while((line[index] != ' ' || in_quotes) && line[index] != '\0' ){
      if(line[index] == '"'){
	in_quotes=!in_quotes;
       	slide(line, index+1, index);    
      }
      else
	index++;
    }
    if(line[index] == ' ')
      line[index++] = '\0';
    else if(line[index] == '\0')      
      args_done = true;

    result[result_index] = &line[temp_start_index];
    result_index++;
  }

  result[num_args] = '\0';

  *argcp = num_args;
  return result;
  
}


/* helper function */
void slide(char *line, int src_index, int dest_index){
  while(line[src_index] != '\0'){
    line[dest_index] = line[src_index];
    dest_index++;
    src_index++;
  }
  line[dest_index]='\0';
}

