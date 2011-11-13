#ifndef _PARSER_H_
#define _PARSER_H_

char ** parsed;

void imprimir_argv(char ** matriz, int tamanho);
int word_count(char * source);
char * readline(void);
char ** parse(char * cmdLine);
void free_parse();

#endif
