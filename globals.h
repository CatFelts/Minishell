/*
 * CS 352 -- Mini Shell
 * globals.h
 *
 * Cat Felts
 * Oct 23, 2016
 * Assignment 4
 */

/*   $Id: globals.h,v 1.1 2016/10/24 00:49:47 feltsc Exp $  */

#pragma once

#ifndef MAIN

#define EXTERN extern

#else

#define EXTERN /*nothing*/

#endif


extern int main_argc;
extern char **main_argv;
