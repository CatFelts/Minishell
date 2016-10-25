/*
 *
 *Cat Felts -- Minishell expand function
 *CS352 Assignment 3 
 * 10/12/16
 *
 */

/*   $Id: expand.c,v 1.12 2016/10/24 00:49:47 feltsc Exp $   */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "proto.h"
#include "globals.h"

/* Helper Function */
/* checks to see if there is a matching end bracket to the start bracking */
int is_validbracks(char *orig, int index){
  while((orig[index]!='\0' && orig[index]!=' ') && orig[index]!='}'){
    index++;
  }
  
  if(orig[index]!='}')
    return -1;
  return index;
}

/* Helper Function */
/* gets the variable name that is inbetween the brackets */
char *get_envvar(char *orig, int st_index, int en_index){
  int envvar_len = en_index - st_index;
  char envvar_name[envvar_len];
  strncpy(envvar_name, orig+st_index+1, envvar_len-1);
  envvar_name[envvar_len-1] = '\0';
  char *envvar_val = getenv(envvar_name);

  return envvar_val;
}

/* Helper Function */
/* counts the number of digits in a number */
int count_digits(int number){
  if(number < 10)
    return 1;
  if(number < 100)
    return 2;
  if(number < 1000)
    return 3;
  if(number < 10000)
    return 4;
  if(number < 100000)
    return 5;
  if(number < 1000000)
    return 6;
  if(number < 10000000)
    return 7;
  if(number < 100000000)
    return 8;
  if(number < 1000000000)
    return 9;
  if(number < 10000000000)
    return 10;
}

/* Helper Function */
/* checks too see if current token is a possible environment variable name */
int is_envvar(char *orig, int *index, char **envvar_p){
  int en_index = *index;
  int st_index = *index;
  int pid;
  int digits=0;
  
  switch(orig[en_index]){

  case '$':
    switch(orig[en_index+1]){

    case '#':
      en_index++;
      *index = en_index;
      digits = count_digits(main_argc-1);
      snprintf(*envvar_p, sizeof(int)*digits, "%d", main_argc-1);
      return 1;                        //replace needs to be called and replace_str is set
      break;

    case '$':
      en_index++;
      *index = en_index;
      pid = getpid();
      digits = count_digits(pid);
      snprintf(*envvar_p, sizeof(int)*digits, "%d", pid);
      return 1;                       //replace needs to be called and replace_str is set
      break;

    case '{':
	st_index++;
	en_index=is_validbracks(orig, en_index+2);
	if(en_index == -1)            //error needs to be thrown: missing end bracket
	  return -1;
	
	*index = en_index;
	*envvar_p = get_envvar(orig, st_index, en_index);
	return 1;                     //replace needs to be called and replace_str is set
	break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      //parse number to find index of correct argument
      return 1;
      break;
     
    default:
      return 0;
      break;
    }
    
    break;

  default:
    return 0;
    break;
  }

}

/* Helper Function */
/* places the replace_str(value of environment variable) into the new buffer */
int replace(char *new,  int newsize, int new_index, char *replace_str){
  int replace_index = 0;

  if(replace_str == NULL)
    return new_index;
  
  while(replace_str[replace_index] != '\0' && new_index < newsize){
    new[new_index] = replace_str[replace_index];
    new_index++;
    replace_index++;
  }
  
  return new_index;
}


int expand(char *orig, char *new, int newsize){
  int new_index= 0;
  int orig_index = 0;
  int replace_code;
  char temp_str[1024];
  char *replace_str = temp_str;

  while(orig[orig_index]!='\0' && new_index < newsize){
    replace_code = is_envvar(orig, &orig_index, &replace_str);
    if(replace_code>0){
      new_index = replace(new, newsize, new_index, replace_str);
      orig_index++;
    }
    else if(replace_code == -1){
      fprintf(stderr, "Error: expand.c: missing end bracket.\n");
      return 0;
    }
    else{
      new[new_index] = orig[orig_index];
      new_index++;
      orig_index++;
    }
  }
  
  if(new_index >= newsize){
    fprintf(stderr, "Error: expand.c: reached end of new buffer.\n");
    return -1;
  }

  new[new_index] = '\0';
  
  return 0;  
}
