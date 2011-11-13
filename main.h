#ifndef _MAIN_H_
#define _MAIN_H_
char * userdir;

int output_r;
int input_r;
char output_r_filename[100];
char input_r_filename[100];
int fd_out;
int fd_in;
pid_t fgChildPid;
int isBackground;
int child_handler_lock;
#endif
