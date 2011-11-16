#ifndef _MAIN_H_
#define _MAIN_H_
#include "list.h"

/**
*  @brief Imprime o prompt na tela (formato copiado do gnome shell).
*  @return void.
*
*/
void printPrompt();

/**
*  @brief Função executada quando usuário manda ctrl+c.
*  @param signum - id do sinal.
*  @return void.
*
*/
void termination_handler(int signum);

/**
*  @brief Função executada quando um processo filho termina.
*
*         Esta função só é útil para processos em background,
*         já que ela fica bloqueada quando um processo roda
*         em foreground.
*  @param signum - id do sinal.
*  @return void.
*
*/
void child_handler(int signum);

/**
*  @brief Função executada quando usuário manda ctrl+z.
*  @param signum - id do sinal.
*  @return void.
*
*/
void sigtstop_handler(int signum);

/**
*  @brief Função que libera memória utilizada antes de terminar
*         o programa.
*  @return void.
*
*/
void free_memory();

char * userdir;
char * username;
char hostname[256];
char * cmdLine;

HISTORY *history;
LIST * cmdList;
LIST * childs;

#endif
