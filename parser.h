#ifndef _PARSER_H_
#define _PARSER_H_
#include "list.h"
/**
*  @brief Conta o número de palavras de uma string.
*  @param source - String para contar.
*  @return Número de palavras da string passada como parâmetro.
*
*/
int word_count(char * source);

/**
*  @brief Lê o comando digitado pelo usuário.
*  @return Comando digitado pelo usuário.
*
*/
char * readline();

/**
*  @brief Adiciona em uma lista o(s) comando(s) digitado(s) pelo usuário.
*
*	  Pega toda a linha de comando digitada e adiciona o comando com
*	  seus parâmetros na lista.
*	  Em caso de uso do pipe, haverá mais de um comando a ser executado,
*	  ela quebrará a linha de comando e colocará cada comando com seus
*	  parâmetros em uma lista.
*  @param cmdList - lista onde os comandos serão armazenados
*  @param cmdLine - linha de comando digitada pelo usuário
*  @return void.
*
*/
void getCmds(LIST *cmdList, char *cmdLine);

/**
*  @brief Recebe a linha de comando e separa os argumentos.
*
* 	  Retira os espaços entre os comandos e seus argumentos,
*	  armazena os comandos e seus parâmetros em um vetor de strings,
*	  na última posição do vetor coloca NULL, isso para o camando ser
*	  interpretado pela função execvp.
*  @param cmdLine - linha de comando que será "parseada".
*  @param command - estrutura que receberá cada comando, com suas flags e parâmetros
*  @return void.
*
*/
void parse(char * cmdLine, COMMAND * command);

#endif
