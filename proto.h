/*
 * CS 352 -- Mini Shell
 * proto.h
 *
 * Cat Felts
 * Oct 1, 2016
 * Assignment 2
 */


#ifndef PROTO_H_
#define PROTO_H_

char **arg_parse(char *line, int *argc);

int check_cmd(char **argv, int argc);

int execute_cmd(char **argv, int argc, int cmd_val);

#endif //PROTO_H_
