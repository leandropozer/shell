#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "main.h"
#include "parser.h"
#include "builtin.h"
#include "list.h"

void printPrompt(char* username, char* hostname)
{
    char path[256];
    getcwd(path, 256);
    if(strcmp(path, userdir) == 0)
        printf("%s@%s:~$ ", username, hostname);
    else if(strstr(path, userdir))
        printf("%s@%s:~%s$ ", username, hostname, path + strlen(userdir));
    else
        printf("%s@%s:%s$ ", username, hostname, path);

}


void termination_handler (int signum)
{
    printf("\n");
    NODE *aux = childs->first;
    while(aux != NULL)
    {
        if (!aux->proc->isBackground)
        {
            kill(-aux->proc->pid, signum);
            break;
        }
        aux = aux->next;
    }
}

void child_handler(int signum)
{
    int status;
    pid_t pid;
    pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
    if((pid >= 0) && (WIFSTOPPED(status) == 0)) ListRemoveByPid(childs, pid);
}

void sigtstop_handler(int signum)
{
    printf("\n");
    if(!ListIsEmpty(childs))
    {
        if (childs->last->proc->pid >= 0)
        {
            ListStopRunningProcessByPid(childs, childs->last->proc->pid);
            kill(childs->last->proc->pid, SIGSTOP);
        }
        else printPrompt(username, hostname);

    }
    else
        printPrompt(username, hostname);
}

int main (int argc, char **argv)
{

    int fd_in;
    int fd_out;
    sigset_t chldMask;
    sigemptyset (&chldMask);
    sigaddset(&chldMask, SIGCHLD);

    struct sigaction new_action, old_action;
    /* Inicializa a lista que guarda os processos que rodam em background */
    childs = malloc(sizeof(LIST));
    ListCreate(childs);
    cmdList = malloc(sizeof(LIST));
    ListCreate(cmdList);
    /* Inicializa a variável que guarda o ID do processo atual do foreground */
    fgChildPid = 0;
    child_handler_lock = 0;
    /* Set up the structure to specify the new action. */
    new_action.sa_handler = termination_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction (SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGINT, &new_action, NULL);
    sigaction (SIGHUP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGHUP, &new_action, NULL);
    sigaction (SIGTERM, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGTERM, &new_action, NULL);

    new_action.sa_handler = child_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction (SIGCHLD, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGCHLD, &new_action, NULL);

    new_action.sa_handler = sigtstop_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction (SIGTSTP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGTSTP, &new_action, NULL);
    /*inicializa lista que guarda o historico de comandos */
    history = malloc(sizeof(struct node));
    history->cmd = NULL;
    history->next = NULL;
    /* Pega o nome do usuario atual e da máquina */
    username = getlogin();
    gethostname(hostname, 256);
    asprintf(&userdir, "/home/%s", username);
    /* Coloca o diretório do usuário como diretório inicial */
    chdir(userdir);
    while (1)
    {
        printPrompt(username, hostname);
        output_r = input_r = 0;
        //if (tcflush(ttyDevice, TCIFLUSH) == 0)
        cmdLine = readline();
        if(strcmp(cmdLine, "") != 0)
        {
            add_history(cmdLine);
            getCmds(cmdList, cmdLine);
            NODE *aux = cmdList->first;
            while (aux != NULL)
            {
                aux->cmd->id = isBuiltIn(aux->cmd->args[0]);
                if(aux->cmd->id >= 0) callBuiltIn(aux->cmd->id, aux->cmd->args);
                else
                {
                    if(aux->next != NULL)
                        pipe2(aux->cmd->pipe, O_CLOEXEC );
                    aux->cmd->pid = fork();
                    if(aux->cmd->pid == 0)
                    {
                        if(aux->prev != NULL || aux->next != NULL)
                        {
                            if(aux->prev == NULL)
                                dup2(aux->cmd->pipe[1], 1);
                            else if(aux->next == NULL)
                                dup2(aux->prev->cmd->pipe[0], 0);
                            else
                            {
                                dup2(aux->prev->cmd->pipe[0], 0);
                                dup2(aux->cmd->pipe[1], 1);
                            }
                            NODE * aux2 = cmdList->first;
                            while(aux2->next != NULL)
                            {
                                close(aux2->cmd->pipe[0]);
                                close(aux2->cmd->pipe[1]);
                                aux2 = aux2->next;
                            }
                        }
                        if(aux->cmd->output_r)
                        {
                            if(aux->cmd->output_r_append)
                                fd_out = open(aux->cmd->output_r_filename, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR, 0666);
                            else
                                fd_out = open(aux->cmd->output_r_filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR, 0666);
                            dup2(fd_out, 1);
                        }
                        if(aux->cmd->input_r)
                        {
                            fd_in = open(aux->cmd->input_r_filename, O_RDONLY, 0666);
                            dup2(fd_in, 0);
                        }
                        signal(SIGTSTP, SIG_IGN);
                        int i = execvp(aux->cmd->args[0], aux->cmd->args);
                        if (aux->cmd->output_r) close(fd_out);
                        if (aux->cmd->input_r) close(fd_in);
                        if(i < 0)
                        {
                            printf("%s: command not found\n", aux->cmd->args[0]);
                            exit(0);
                        }
                    }
                }
                aux = aux->next;
            }
            aux = cmdList->first;
            while (aux->next != NULL)
            {
                close(aux->cmd->pipe[0]);
                close(aux->cmd->pipe[1]);
                aux = aux->next;
            }
            aux = cmdList->first;
            int count = 0;
            while (aux != NULL)
            {
                if(isBuiltIn(aux->cmd->args[0]) == -1)
                {
                    if(aux->cmd->isBackground == 0) count++;
                    setpgid(aux->cmd->pid, cmdList->first->cmd->pid);
                    PROCESS * p;
                    p = malloc(sizeof(PROCESS));
                    p->pid = aux->cmd->pid;
                    p->isBackground = aux->cmd->isBackground;
                    strcpy(p->status, "Running");
                    strcpy(p->command, aux->cmd->args[0]);
                    sigprocmask(SIG_BLOCK, &chldMask, NULL);
                    ListInsert(childs, p, NULL);
                    sigprocmask(SIG_UNBLOCK, &chldMask, NULL);
                }
                aux = aux->next;
            }
            int i;
            int status;
            sigprocmask(SIG_BLOCK, &chldMask, NULL);
            for (i = 0; i < count; i++)
            {
                pid_t pidfg = waitpid(-cmdList->first->cmd->pid, &status, WUNTRACED);
                if ((pidfg >= 0) && (WIFSTOPPED(status) == 0)) ListRemoveByPid(childs, pidfg);
            }
            sigprocmask(SIG_UNBLOCK, &chldMask, NULL);
        }
        ListPurgeCmds(cmdList);
        free(cmdLine);
    }
}





