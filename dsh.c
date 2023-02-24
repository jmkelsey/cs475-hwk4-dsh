/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>


// TODO: Your function definitions (declarations in dsh.h)

//splits a string into an array of strings by token delim
char** split(char *str, char *delim) {
    int numTokens = 0;
    char strCopy[strlen(str) + 1];
    strcpy(strCopy, str);
    char *token = strtok(str, delim);

    // count the number of tokens
    while (token != NULL) {
        numTokens++;
        token = strtok(NULL, delim);
    }
    // allocate memory for the result array
    char **result = malloc((numTokens+1) * sizeof(char*));
    // reset the string pointer
    token = strtok(strCopy, delim);
    // loop through the tokens and copy them into the result array
    int i = 0;
    while (token != NULL) {
        result[i] = malloc((strlen(token)+1) * sizeof(char));
        strcpy(result[i], token);
        token = strtok(NULL, delim);
        i++;
    }
    // set the last element to NULL to mark the end of the array
    
    result[numTokens+1] = NULL;
    return result;
}

//prompts the user for input and stores that in a string (removes newline at the end and replaces with '\0')
char* promptUser(char *printStatement) {
    char *input = (char*) calloc(MAXBUF,sizeof(char));
    int validInput = 0;
    while (!validInput) {
        //print dhs
        printf("%s", printStatement);
        //read input
        fgets(input, MAXBUF, stdin);
        //remove newline
        if (input[strlen(input)-1] == '\n') {
            input[strlen(input)-1] = '\0';
        }
        //check if it's only spaces or empty
        int allSpace = 1;
        for (int i = 0; i < strlen(input); i++) {
            if (input[i] != ' ' && input[i] != '\t') {
                allSpace = 0;
                break;
            }
        }
        if (!allSpace) {
            //if it's not empty return it
            validInput = 1;
        }
    }
    return input;
}

int runExe(char **args) {
    pid_t pid;
    int background = 0;

    //check if it should be run in background
    int i = 0;
    while (args[i] != NULL) {
        i++;
    }
    if (i > 0 && strcmp(args[i-1], "&") == 0) {
        background = 1;
        args[i-1] = NULL;
    }

    pid = fork();
    if (pid == 0) {
        //we are the child!
        if (execv(args[0], args) == -1) {
            perror("bad fork");
        }
    } else if (pid < 0) {
        perror("bad fork");
    } else {
        //we are the parent!
        if (!background) {
            wait(NULL);
        }
    }
    printf("\n");
    return 0;
}

int mode2(char **args) {
    if (checkCmd(args[0],args) == 1) {
        //
        printf("\n");
        return 0;
    }
    //set up current working dir
    char *cwd = (char*) calloc(MAXBUF,sizeof(char));
    getcwd(cwd,BUFSIZ);
    //set up file path
    char *filePath = (char*) calloc(MAXBUF,sizeof(char));
    strcpy(filePath,cwd);
    strcat(filePath,"/");
    strcat(filePath,args[0]);
    //file path test

    //test if it's in cwd
    if (fopen(filePath, "r") != NULL) {
        args[0] = filePath;
        runExe(args);
        return 0;
    } else {
        //we need to find the rest of the paths
        char **altPaths = split(getenv("PATH"),":");
        for (int i = 0; altPaths[i] != NULL; i++) {
            if (strlen(altPaths[i]) < MAXBUF) {
                strcpy(filePath,altPaths[i]);
                strcat(filePath,"/");
                strcat(filePath,args[0]);
                if (fopen(filePath,"r") != NULL) {
                    args[0] = filePath;
                    runExe(args);
                    return 0;
                }
                
            }
        }
        printf("No command of that name found\n");
    }
    return 0;
}

int printTokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        for (int j = 0; tokens[i][j] != '\0'; j++) {
            printf("%c", tokens[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int checkCmd(char *cmd, char **args) {
    if (!strcmp(cmd, "cd")) {
        chdir(args[1]);
		return 1;
    } else if (!strcmp(cmd, "exit")) {
        exit(0);
		return 1;
	} else if (!strcmp(cmd, "pwd")) {
        char *cwd = (char*) calloc(MAXBUF,sizeof(char));
        getcwd(cwd,BUFSIZ);
        printf("%s\n",cwd);
		return 1;
	} else if (!strcmp(cmd, "history")) {
		return 1;
	} else if (!strcmp(cmd, "echo")) {
		return 1;
    }
	return 0;
}