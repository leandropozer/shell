#ifndef _MAIN_H_
#define _MAIN_H_
#include "list.h"

void printPrompt(char* username, char* hostname);
void termination_handler(int signum);
void child_handler(int signum);
void sigtstop_handler(int signum);
void free_memory();

char * userdir;
char * username;
char hostname[256];
char * cmdLine;

HISTORY *history;
LIST * cmdList;
LIST * childs;

#endif
