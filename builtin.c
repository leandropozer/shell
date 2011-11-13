#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "main.h"
#include "builtin.h"
#include "parser.h"
#include "lista.h"

int isBuiltIn(char * cmd)
{
    char * commands[] = {"cd", "history", "pwd", "exit", "jobs", "bg", "fg", "kill", "echo"};
    int i;
    for (i = 0; i < 9; i++)
        if (strcmp(commands[i], cmd) == 0) return i;
    return -1;
}

void callBuiltIn(int cmd_id, char * arg)
{
    switch(cmd_id)
    {
    case 0:
        cd();
        break;
    case 1:
        print_history();
        break;
    case 2:
        pwd();
        break;
    case 3:
        exit(0);
        break;
    case 4:
        jobs();
        break;
    case 5:
        bg(arg);
        break;
    case 6:
        fg(arg);
        break;
    case 7:
        kill_cmd(arg);
        break;
    default:
        break;
    }
}

void add_history(char * cmd)
{
    struct node *tmp = history->next;
    struct node *prev = history;
    if(history->cmd == NULL)
    {
        history->cmd = malloc(strlen(cmd) + 1);
        strcpy(history->cmd, cmd);
    }
    else
    {
        while (tmp != NULL)
        {
            prev = tmp;
            tmp = tmp->next;
        }
        tmp = malloc(sizeof(struct node));
        tmp->cmd = malloc(strlen(cmd));
        tmp->next = NULL;
        strcpy(tmp->cmd, cmd);
        prev->next = tmp;
    }
}

void print_history()
{
    struct node *tmp = history;
    int i = 1;
    printf("%d %s\n", i, tmp->cmd);
    while(tmp->next != NULL)
    {
        i++;
        tmp = tmp->next;
        printf("%d %s\n", i, tmp->cmd);
    }
}

void cd()
{
    int error;
    if(parsed[1] == NULL) error = chdir(userdir);
    else error = chdir(parsed[1]);
    if (error ==  -1) printf("bash: cd: %s: %s\n", parsed[1],strerror(errno));
}

void pwd()
{
    char path[256];
    getcwd(path, 256);
    printf("%s\n", path);
}

void jobs()
{
    NODE *aux = childs->first;
    ITEM * item;
    while (aux != NULL)
    {
        item = aux->item;
        printf("[%d] %s %s\n",item->pid, item->status, item->command);
        aux = aux->next;
    }
}

void bg(char * arg)
{
    if(!ListIsEmpty(childs)) {
        char * end;
        int n;
        pid_t pid;
        if(arg == NULL)
            pid = ListLastStoppedToBg(childs);
        else {
            n = strtol(arg, &end, 10);
            printf("n: %d", n);
            pid = ListToBg(childs, n);
        }
        printf("pid escolhido: %d", pid);
        kill(pid, SIGCONT);
    }
}

void fg(char * arg)
{
    if(!ListIsEmpty(childs)) {
        char * end;
        int n;
        pid_t pid, pidfg;
        int status;
        if(arg == NULL)
            pid = ListLastToFg(childs);
        else {
            n = strtol(arg, &end, 10);
            pid = ListToFg(childs, n);
        }
        child_handler_lock = 1;
        pidfg = waitpid(pid, &status, WUNTRACED);
        child_handler_lock = 0;
        if ((pidfg >= 0) && (WIFSTOPPED(status) == 0)) ListRemoveByPid(childs, childPid);
    }
}

void kill_cmd(char * arg)
{
    if(!ListIsEmpty(childs)) {
        char * end;
        int n;
        NODE * aux = childs->last;
        if(arg == NULL) n = aux->item->pid;
        else n = strtol(arg, &end, 10);
            kill(n, SIGTERM);
    }

}
