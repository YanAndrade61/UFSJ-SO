#ifndef __SHELLSO_H__
#define __SHELLSO_H__

#include "fat.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS 3
#define MAX_CMD 1024

int shell();
void help();
int readInput(char* cmd_line);
char **processInput(char *cmd);
int builtinCommands(char **commands);


#endif