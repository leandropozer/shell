#ifndef _LISTA_H_
#define _LISTA_H_
typedef struct
{
    pid_t pid;
    char command[256];
    char status[100];
    int isBackground;
} ITEM;

typedef struct NODE
{
    ITEM *item;
    struct NODE *next;
    struct NODE *prev;
} NODE;

typedef struct
{
    NODE *first;
    NODE *last;
} LIST;

void ListCreate(LIST *list);
int ListIsEmpty(LIST *list);
int ListInsert(LIST *list, ITEM *item);
int ListGetByPid(LIST *list, pid_t pid, ITEM *item);
int ListGetRunningProcess(LIST *list, ITEM *item);
int ListRemoveByPid(LIST *list, pid_t pid);
pid_t ListLastStoppedToBg(LIST *list);
pid_t ListToBg(LIST *list, pid_t n);
pid_t ListLastToFg(LIST *list);
pid_t ListToFg(LIST *list, pid_t n);

LIST * childs;
#endif
