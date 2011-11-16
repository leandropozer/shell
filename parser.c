#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "main.h"

void imprimir_argv(char ** matriz, int tamanho)
{
    int i = 0;
    for(i = 0; i <= tamanho; i++)
    {
        printf("%s ", matriz[i]);
    }
}

int word_count(char * source)
{
    int i;
    int sp = 1;
    int words = 0;
    for (i = 0; source[i] != '\0'; i++)
    {
        if(isspace(source[i])) sp=1;
        else if(sp)
        {
            ++words;
            sp=0;
        }
    }
    return words;
}

void getCmds(LIST *cmdList, char *cmdLine)
{
    char *cmds;
    char * saveptr;
    cmds = strtok_r(cmdLine, "|", &saveptr);
    while(cmds != NULL)
    {
        COMMAND * cmd = malloc(sizeof(COMMAND));
        initCommand(cmd);
        parse(cmds, cmd);
        ListInsert(cmdList, NULL, cmd);
        cmds = strtok_r(NULL, "|", &saveptr);
    }
}


void parse(char * cmdLine, COMMAND * cmd)
{
    char * cmds;
    int i = 0;
    char *saveptr;
    int cmd_len;
    cmd_len = word_count(cmdLine);
    cmd->args = (char**)malloc((cmd_len+1)*sizeof(char*));
    cmds = strtok_r(cmdLine, " ", &saveptr);
    while(cmds != NULL)
    {
        if((strcmp("&", cmds) == 0))
        {
            cmd->isBackground = 1;
        }
        else if((strcmp("<", cmds) == 0))
        {
            cmd->input_r = 1;
            cmd->input_r_filename = strtok_r(NULL, " ", &saveptr);
        }
        else if((strcmp(">", cmds) == 0))
        {
            cmd->output_r = 1;
            cmd->output_r_filename = strtok_r(NULL, " ", &saveptr);
        }
        else if((strcmp(">>", cmds) == 0))
        {
            cmd->output_r_append = 1;
            cmd->output_r = 1;
            cmd->output_r_filename = strtok_r(NULL, " ", &saveptr);
        }
        else
        {
            cmd->args[i] = malloc((strlen(cmds)+1)*sizeof(char));
            strcpy(cmd->args[i], cmds);
            i++;
        }
        cmds = strtok_r(NULL, " ", &saveptr);

    }
    cmd->args[i] = (char *)0;
    cmd->args = (char**)realloc(cmd->args, (i+1)*sizeof(char*));
    cmd->size = i;
}

char* readline()
{
    char * input = (char *)malloc(sizeof(char) * 100);
    char c;
    int i = 0;
    c = getchar();
    while (c != '\n')
    {
        if(c > 31) //Com esta condição, evitamos lixo na entrada.
        {
            input[i] = c;
            i++;
        }
        c = getchar();
    }
    input[i] = '\0';
    return input;
}
