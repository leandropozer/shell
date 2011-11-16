#ifndef _BUILTIN_H_
#define _BUILTIN_H_


int isBuiltIn(char * cmd);
void callBuiltIn(int cmd_id, COMMAND *cmd);
void add_history(char * cmd);
void print_history(COMMAND *cmd);
void free_history();
void cd(char * arg);
void pwd(COMMAND *cmd);
void jobs(COMMAND *cmd);
void bg(char * arg);
void fg(char * arg);
void kill_cmd(char * arg);
void echo (COMMAND *cmd);
void about(COMMAND *cmd);
#endif

