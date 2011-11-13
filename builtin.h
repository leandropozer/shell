#ifndef _BUILTIN_H_
#define _BUILTIN_H_

struct node
{
    char * cmd;
    struct node *next;
};

struct node *history;

pid_t childPid;
int isBuiltIn(char * cmd);
void callBuiltIn(int cmd_id, char * arg);
void add_history(char * cmd);
void print_history();
void cd();
void pwd();
void jobs();
void bg(char * arg);
void fg(char * arg);
void kill_cmd(char * arg);
#endif

