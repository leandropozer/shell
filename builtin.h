#ifndef _BUILTIN_H_
#define _BUILTIN_H_

/**
*  @brief Verifica se o comando é BuiltIn
*
*  Se o comando for BuiltIn, retorna seu id para ser executado
*  posteriormente.
*  @param cmd - comando a ser analisado.
*  @return Valor do id do comando.
*  @retval -1 Se o comando não é BuiltIn.
*
*/
int isBuiltIn(char * cmd);

/**
*  @brief Executa o comando BuiltIn.
*  @param cmd_id - id do comando a ser executado, retorno da função isBuiltIn.
*  @param cmd - Estrutura com informações do comando.
*  @return void.
*
*/
void callBuiltIn(int cmd_id, COMMAND *cmd);

/**
*  @brief Adiciona o comando do usuário na lista do histórico.
*  @param cmd - Comando a ser adicionado.
*  @return void.
*
*/
void add_history(char * cmd);

/**
*  @brief Imprime os comando armazenados no histórico.
*  @param cmd - Estrutura com possível parâmetro do comando, representa
*         quantos comandos deseja imprimir, se for NULL imprime os últimos
*         10, que é o valor default.
*  @return void.
*
*/
void print_history(COMMAND *cmd);

/**
*  @brief Libera memória alocada pelo histórico.
*  @return void.
*
*/
void free_history();

/**
*  @brief Muda o diretório atual no qual o sistema está trabalhando.
*  @param arg - Diretorio para o qual deseja mudar.
*  @return void.
*
*/
void cd(char * arg);

/**
*  @brief Mostra o diretório no qual o sistema está trabalhando.
*  @param cmd - Estrutura com informações do comando. (Necessário para
*         redirecionamento de saída).
*  @return void.
*
*/
void pwd(COMMAND *cmd);

/**
*  @brief Mostra uma lista com os jobs atuais.
*  @param cmd - Estrutura com informações do comando. (Necessário para
*         redirecionamento de saída).
*  @return void.
*
*/
void jobs(COMMAND *cmd);

/**
*  @brief Coloca um processo em background.
*  @param arg - Id do processo a ser colocado em background, se for
*         NULL o processo mais recente é colocado em foreground.
*  @return void.
*
*/
void bg(char * arg);

/**
*  @brief Coloca um processo suspenso em foreground.
*  @param arg - Id do processo a ser colocado em foreground, se for
*         NULL o processo suspenso mais recente é colocado em foreground.
*  @return void.
*
*/
void fg(char * arg);

/**
*  @brief Manda o sinal SIGTERM para um processo filho
*  @param arg - Id do processo a receber o sinal, se for NULL o processo
*         mais recente recebe o sinal.
*  @return void.
*
*/
void kill_cmd(char * arg);

/**
*  @brief imprime os argumentos enviados na tela.
*  @param cmd - Estrutura com informações do comando. Daqui se extrai os
*         argumentos a serem imprimidos. (Também necessário para
*         redirecionamento de saída).
*  @return void.
*
*/
void echo (COMMAND *cmd);

/**
*  @brief Comando simples que imprime informações sobre os autores.
*  @param cmd - Estrutura com informações do comando. (Necessário para
*         redirecionamento de saída).
*  @return void.
*
*/
void about(COMMAND *cmd);
#endif
