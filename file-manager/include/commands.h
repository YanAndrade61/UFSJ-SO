#ifndef __COMMANDS_H__
#define __COMMANDS_H__

void init(void);
void load();
int  ls(char** argv);
int  ulink(char** argv);
int  create(char** argv, int attr);
int  cwrite(char** argv);
int  append(char** argv);
int  cread(char** argv);

#endif