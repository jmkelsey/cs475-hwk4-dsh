/*
 * dsh.h
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */

#define MAXBUF 256

// TODO: Your function prototypes below
char* promptUser();
char** split(char *str, char *delim);
int runExe(char **args);
int mode2(char **args);
int printTokens(char **tokens);
int checkCmd(char *cmd, char **args);