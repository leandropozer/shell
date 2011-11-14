#ifndef _LISTA_H_
#define _LISTA_H_
typedef struct
{
    pid_t pid;
    char command[256];
    char status[100];
    int isBackground;
} PROCESS;

typedef struct
{
    char **args;
    int isBackground;
    int input_r;
    int output_r;
    int output_r_append;
    int size;
    char * output_r_filename;
    char * input_r_filename;
    int id;
    pid_t pid;
} COMMAND;

typedef struct NODE
{
    PROCESS *proc;
    COMMAND *cmd;
    struct NODE *next;
    struct NODE *prev;
} NODE;

typedef struct
{
    NODE *first;
    NODE *last;
} LIST;

void ListCreate(LIST *list);
void initCommand(COMMAND *cmd);
int ListIsEmpty(LIST *list);
int ListInsert(LIST *list, PROCESS *proc, COMMAND *cmd);
int ListGetByPid(LIST *list, pid_t pid, PROCESS *proc);
int ListStopRunningProcessByPid(LIST *list,pid_t pid);
int ListRemoveByPid(LIST *list, pid_t pid);
pid_t ListLastStoppedToBg(LIST *list);
pid_t ListToBg(LIST *list, pid_t n);
pid_t ListLastToFg(LIST *list);
pid_t ListToFg(LIST *list, pid_t n);
void ListPurgeCmds(LIST *list);
LIST * childs;
#endif
