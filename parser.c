#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "main.h"

void imprimir_argv(char ** matriz, int tamanho){
        int i = 0;
        for(i = 0; i <= tamanho; i++){
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

char ** parse(char * cmdLine) {
    cmd_len = word_count(cmdLine);
    char * cmds;
    int i;
    parsed = (char**)malloc((cmd_len+1)*sizeof(char*));
    cmds = strtok(cmdLine, " ");

    for(i = 0; cmds != NULL; i++) {
        if((strcmp("&", cmds) == 0)) {
            isBackground = 1;
            cmd_len--;
        }
        else if((strcmp("<", cmds) == 0)) {
            input_r = 1;
            cmd_len--;
            input_r_filename = strtok(NULL, " ");
        }
        else if((strcmp(">", cmds) == 0)) {
            output_r = 1;
            cmd_len--;
            output_r_filename = strtok(NULL, " ");
        }
        else if((strcmp(">>", cmds) == 0)) {
            output_r_append = 1;
            output_r = 1;
            cmd_len--;
            output_r_filename = strtok(NULL, " ");
        }
        else {
            parsed[i] = malloc(strlen(cmds));
            strcpy(parsed[i], cmds);
        }
            cmds = strtok(NULL, " ");

    }
    parsed[cmd_len] = (char *)0;
    //imprimir_argv(parsed, words);
    return parsed;
}

void free_parse() {
    int i;
    for(i = 0; parsed[i] != NULL; i++) free(parsed[i]);
    free(parsed);
}

char* readline()
{
    char * input = (char *)malloc(sizeof(char) * 100);
    char c;
    int i = 0;
    c = getchar();
    while (c != '\n')
    {
        if((c > 31) && (c < 172)) {
            input[i] = c;
            i++;
        }
        c = getchar();
    }
    input[i] = '\0';
    return input;
}
