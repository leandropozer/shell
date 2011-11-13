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
    int words = word_count(cmdLine);
    char * cmds;
<<<<<<< HEAD
    int flag_out1 = 0;
    int flag_out2 = 0;
    int flag_in1 = 0;
    int flag_in2 = 0;
    int i, cc;
=======
    int i;
>>>>>>> a9f57c0592aa1173987aafd6cd59ed341503043a
    parsed = (char**)malloc((words+1)*sizeof(char*));
    cmds = strtok(cmdLine, " ");

    for(i = 0; cmds != NULL; i++) {
<<<<<<< HEAD
	
	if(flag_out1 == 1) flag_out2 = 1;
        if(flag_in1 == 1) flag_in2 = 1;
	
=======
>>>>>>> a9f57c0592aa1173987aafd6cd59ed341503043a
        if((strcmp("&", cmds) == 0)) {
            isBackground = 1;
            words--;
        }
        else {
            parsed[i] = malloc(strlen(cmds));
<<<<<<< HEAD
	    if(flag_out1 != 1 && flag_in1 != 1){
            strcpy(parsed[i], cmds);
        }
            
        

	if(!strcmp(cmds,">")){
            output_r = 1;
            flag_out1 = 1;
            cc = i;
        }
        if(!strcmp(cmds,"<")){
            input_r = 1;
            flag_in1 = 1;
            cc = i;
        }

	if(flag_out2 == 1){
            strcpy(output_r_filename,cmds);
        }
        if(flag_in2 == 1){
            strcpy(input_r_filename,cmds);
        }

	}
	
            cmds = strtok(NULL, " ");

    }
    if(flag_out1 == 0 && flag_in1 == 0) cc = words;
    parsed[cc] = (char *)0;
=======
            strcpy(parsed[i], cmds);
        }
            cmds = strtok(NULL, " ");

    }
    parsed[words] = (char *)0;
>>>>>>> a9f57c0592aa1173987aafd6cd59ed341503043a
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
        input[i] = c;
        i++;
        c = getchar();
    }
    input[i] = '\0';
    return input;
}
