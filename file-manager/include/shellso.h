#ifndef __SHELLSO_H__
#define __SHELLSO_H__


#define MAX_ARGS 3
#define MAX_CMD 1024

int  shell();
void help();
int  readInput(char* cmd_line);
char **parserStr(char *str, char* sep, int* sz);
int  builtinCommands(char **argv, int argc);
void erro(int err);




#endif