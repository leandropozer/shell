#ifndef _PARSER_H_
#define _PARSER_H_
#include "list.h"
char ** parsed;

int cmd_len;
void imprimir_argv(char ** matriz, int tamanho);
int word_count(char * source);
char * readline(void);
void getCmds(LIST *cmdList, char *cmdLine);
void parse(char * cmdLine, COMMAND * command);
void free_parse();

#endif
