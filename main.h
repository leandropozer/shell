#ifndef _MAIN_H_
#define _MAIN_H_
#include "list.h"
char * userdir;

int output_r;
int output_r_append;
int input_r;
char * output_r_filename;
char * input_r_filename;
char field0_name[100];
char field1_name[100];
int is_pipe;
pid_t fgChildPid;
int isBackground;
int child_handler_lock;
char * username;
char hostname[256];
char * cmdLine;
LIST * cmdList;

void free_memory();

#endif
