#ifndef _BUILTIN_H_
#define _BUILTIN_H_


int isBuiltIn(char * cmd);
void callBuiltIn(int cmd_id, COMMAND *cmd);
void add_history(char * cmd);
void print_history(char * arg);
void free_history();
void cd(char * arg);
void pwd();
void jobs();
void bg(char * arg);
void fg(char * arg);
void kill_cmd(char * arg);
void echo (COMMAND *cmd);
#endif

