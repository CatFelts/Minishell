/* CS 352 -- Mini Shell!  
 *
 *   Sept 27, 2016,  Cat Felts
 *   Assignment 1
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>


/* Constants */

#define LINELEN 1024

/* Prototypes */

void processline (char *line);

/* Shell main */

int
main (void)
{
  char   buffer [LINELEN];
  int    len;

  while (1) {

    /* prompt and get line */
    fprintf (stderr, "%% ");
    if (fgets (buffer, LINELEN, stdin) != buffer)
      break;

    /* Get rid of \n at end of buffer. */
    len = strlen(buffer);
    if (buffer[len-1] == '\n')
      buffer[len-1] = 0;

    /* Run it ... */
    processline (buffer);

  }

  if (!feof(stdin))
    perror ("read");

  return 0;/* Also known as exit (0); */
}

char ** arg_parse(char *line)
{
  int index = 0;
  int num_args = 0;
  int temp_start_index = 0;
  int result_index = 0;
  bool args_counted = false;
  bool args_done = false;

  /* first parse: count arguments */
  while(!args_counted){

    /* skip leading whitespace */
    while(line[index] == ' ')
      index++;

    /* walk thru word */
    while(line[index] != ' ' && line[index] != '\0'){
      index++;
    }
    num_args++;
    if(line[index] == '\0')
      args_counted = true;
  }

  /* second parse: assign pointers and insert null terminators */
  index = 0;
  char **result = (char **)malloc((sizeof(char *))*(num_args+1));
  if(result == NULL){
    printf("Error allocating memory!\n");
    return NULL;                                                         //not sure what to return here. needs to be of type char **
  }
  
  while(!args_done){

    /* skip leading whitespace */
    while(line[index] == ' ')
      index++;

    /* attach null terminator to end of word and assign pointer to beginning of word */
    temp_start_index = index;
    
    while(line[index] != ' ' && line[index] != '\0'){
      index++;
    }
    if(line[index] == ' ')
      line[index] = '\0';
    else if(line[index] == '\0')       //reached end of line
      args_done = true;
    index++;

    result[result_index] = &line[temp_start_index];
    result_index++;
  }


  return result;
  
}


void processline (char *line)
{
  pid_t  cpid;
  int    status;

  /* parse line to find commands and arguments */
  char **line_args = arg_parse(line);
  if(line_args==NULL){
    perror("arg_parse");
    return;
  }

  /* Start a new process to do the job. */
  cpid = fork();
  if (cpid < 0) {
    perror ("fork");
    return;
  }

  /* Check for who we are! */
  if (cpid == 0) {
    /* We are the child! */
    execvp (*line_args, line_args);
    perror ("exec");
    exit (127);
  }

  /* Have the parent wait for child to complete */
  if (wait (&status) < 0)
    perror ("wait");

  free(line_args);
}

