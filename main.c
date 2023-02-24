/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"

int main(int argc, char **argv)
{
    while (1) {
        char *inputStr = promptUser("dhs> ");
        char **tokens = split(inputStr, " ");
        if (tokens[0][0] == '/' || (tokens[0][0] == '.' && tokens[0][1] == '/')) {
            runExe(tokens);
        } else {
            mode2(tokens);
        }
        free(inputStr);
        free(tokens);
    }
    
    return 0;
	
}