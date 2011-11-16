#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <unistd.h>
#include "main.h"
#include "builtin.h"
#include "parser.h"
#include "list.h"

int isBuiltIn(char * cmd)
{
    char * commands[] = {"cd", "history", "pwd", "exit", "jobs", "bg", "fg", "kill", "echo", "about"};
    int i;
    for (i = 0; i < 10; i++)
        if (strcmp(commands[i], cmd) == 0) return i;
    return -1;
}

void callBuiltIn(int cmd_id, char ** arg)
{
    switch(cmd_id)
    {
    case 0:
        cd(arg[1]);
        break;
    case 1:
        print_history(arg[1]);
        break;
    case 2:
        pwd();
        break;
    case 3:
        ListPurgeCmds(cmdList);
        free(cmdLine);
        free_memory();
        exit(0);
        break;
    case 4:
        jobs();
        break;
    case 5:
        bg(arg[1]);
        break;
    case 6:
        fg(arg[1]);
        break;
    case 7:
        kill_cmd(arg[1]);
        break;
    case 8:
        echo(arg);
        break;
    case 9:
        printf("Shell para trabalho de SSC0141.\nDesenvolvido por Lucas Lobosque e Leandro Pozer.\n");
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
        history->cmd = malloc((strlen(cmd) + 1)*sizeof(char));
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
        tmp->cmd = malloc((strlen(cmd) + 1)*sizeof(char));
        tmp->next = NULL;
        strcpy(tmp->cmd, cmd);
        prev->next = tmp;
    }
}

void print_history(char * arg)
{
    int n = 10;
    char * end;
    if (arg != NULL) n = strtol(arg, &end, 10);
    struct node *tmp = history;
    int i = 1;
    printf("%d %s\n", i, tmp->cmd);
    while(tmp->next != NULL && (i < n))
    {
        i++;
        tmp = tmp->next;
        printf("%d %s\n", i, tmp->cmd);
    }
}

void free_history()
{
    struct node *tmp = history;
    while(tmp != NULL)
    {
        free(tmp->cmd);
        tmp = tmp->next;
    }
    free(history);
}

void cd(char * arg)
{
    int error;
    if(arg == NULL) error = chdir(userdir);
    else error = chdir(arg);
    if (error ==  -1) printf("bash: cd: %s: %s\n", arg,strerror(errno));
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
    PROCESS * proc;
    while (aux != NULL)
    {
        proc = aux->proc;
        printf("[%d] %s %s\n",proc->pid, proc->status, proc->command);
        aux = aux->next;
    }
}

void bg(char * arg)
{
    if(!ListIsEmpty(childs))
    {
        int n;
        PROCESS *process;
        if(arg == NULL)
            process = ListGetLastStopped(childs);
        else
        {
            n = strtol(arg, NULL, 10);
            process = ListGetProcess(childs, n);
        }
        if (process)
            if(kill(process->pid, SIGCONT) == 0)
            {
                process->isBackground = 1;
                strcpy(process->status, "Running");
            }
    }
}

void fg(char * arg)
{
    int n;
    pid_t pidfg;
    int status;
    sigset_t chldMask;
    sigemptyset (&chldMask);
    sigaddset(&chldMask, SIGCHLD);
    PROCESS * process;
    if(!ListIsEmpty(childs))
    {
        if(arg == NULL)
            process = childs->first->proc;
        else
        {
            n = strtol(arg, NULL, 10);
            process = ListGetProcess(childs, n);
        }
        if(process)
            if(kill(process->pid, SIGCONT) == 0) {
                printf("entrou aqui. pid - %d\n", process->pid);
                process->isBackground = 0;
                strcpy(process->status, "Running");
                sigprocmask(SIG_BLOCK, &chldMask, NULL);
                pidfg = waitpid(process->pid, &status, WUNTRACED);
                if ((pidfg >= 0) && (WIFSTOPPED(status) == 0)) ListRemoveByPid(childs, pidfg);
                sigprocmask(SIG_UNBLOCK, &chldMask, NULL);
            }

    }
}

void kill_cmd(char * arg)
{
    if(!ListIsEmpty(childs))
    {
        char * end;
        int n;
        NODE * aux = childs->last;
        if(arg == NULL) n = aux->proc->pid;
        else n = strtol(arg, &end, 10);
        kill(n, SIGTERM);
    }
}

void echo(char ** cmds)
{
    int i;
    for(i = 1; i < cmd_len; i++)
    {
        printf("%s ", cmds[i]);
    }
    if (cmd_len > 1) printf("\n");
}
