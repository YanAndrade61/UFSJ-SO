#ifndef __COMMANDS_H__
#define __COMMANDS_H__

void init(void);
void load();
int  ls(char** argv, int argc);
int  ulink(char** argv, int argc);
int  create(char** argv, int argc, int attr);
int  cwrite(char** argv, int argc);
int  append(char** argv, int argc);
int  cread(char** argv, int argc);

#endif