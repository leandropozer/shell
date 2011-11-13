#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "main.h"
#include "parser.h"
#include "builtin.h"
#include "lista.h"

void printPrompt(char* username, char* hostname)
{
    char path[256];
    getcwd(path, 256);
    if(strcmp(path, userdir) == 0)
        printf("%s@%s:~$ ", username, hostname);
    else
        printf("%s@%s:%s$ ", username, hostname, path);
}

void execute(char ** cmd)
{

}

void termination_handler (int signum)
{
    if (fgChildPid != 0)
    {
        kill(fgChildPid, signum);
        fgChildPid = 0;
    }
}

void child_handler(int signum)
{
    if(child_handler_lock == 0) {
        int status;
        pid_t pid;
        pid = waitpid(0, &status, WNOHANG|WUNTRACED);
        if((pid >= 0) && (WIFSTOPPED(status) == 0)) ListRemoveByPid(childs, pid);
    }

}

void sigtstop_handler(int signum) {
    ITEM *item;
    item = malloc(sizeof(ITEM));
    ListGetRunningProcess(childs, item);
    kill(childPid, SIGSTOP);
}

int main (int argc, char **argv)
{


    char * cmdLine;
    char * username;
    char ** cmd;
    char hostname[256];
    size_t len = 256;
    struct sigaction new_action, old_action;

    /* Inicializa a lista que guarda os processos que rodam em background */
    childs = malloc(sizeof(LIST));
    ListCreate(childs);
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
    gethostname(hostname, len);
    asprintf(&userdir, "/home/%s", username);
    /* Coloca o diretório do usuário como diretório inicial */
    chdir(userdir);
    while (1)
    {
        printPrompt(username, hostname);
        output_r = input_r = 0;
        rewind(stdin);
        cmdLine = readline();
        if(strcmp(cmdLine, "") != 0)
        {
            add_history(cmdLine);
            cmd = parse(cmdLine);
            int cmd_id = isBuiltIn(cmd[0]);
            if(cmd_id >= 0) callBuiltIn(cmd_id, cmd[1]);
            else
            {
                if((childPid = fork()) == 0)
                {
                    if(output_r)
                    {
                        fd_out = open(output_r_filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR, 0666);
                        dup2(fd_out, 1);
                    }
                    if(input_r)
                    {
                        fd_in = open(input_r_filename, O_RDONLY, 0666);
                        dup2(fd_in, 0);
                    }
                    fgChildPid = childPid;
                    int i = execvp(cmd[0], cmd);
                    if (output_r) close(fd_out);
                    if (input_r) close(fd_in);
                    if(i < 0)
                    {
                        printf("%s: command not found\n", cmd[0]);
                        exit(101);
                    }
                }
                else
                {
                    /*Após criar o processo filho, o pai insere em uma lista ligada o novo processo */
                    int status;
                    pid_t pidfg;
                    ITEM * p;
                    p = malloc(sizeof(ITEM));
                    p->pid = childPid;
                    p->isBackground = isBackground;
                    strcpy(p->status, "Running");
                    strcpy(p->command, cmdLine);
                    ListInsert(childs, p);
                    isBackground = 0;
                    /*E espera ele terminar, caso seja um processo de foreground */
                    if(!p->isBackground) {
                        child_handler_lock = 1;
                        pidfg = waitpid(childPid, &status, WUNTRACED);
                        child_handler_lock = 0;
                        if ((pidfg >= 0) && (WIFSTOPPED(status) == 0)) ListRemoveByPid(childs, childPid);
                    }
                }
            }
            free_parse();
            free(cmdLine);
        }
    }
}
