#ifndef _LISTA_H_
#define _LISTA_H_

/** @brief Lista simples para uso do comando builtin history.
*
*/
typedef struct history
{
    char * cmd;
    struct history *next;
} HISTORY;

/** @brief Estrutura do processo.
*
*          Esta estrutura guarda todas as informações
*          pertinentes de um processo.
*
*/
typedef struct
{
    pid_t pid;
    char command[256];
    char status[100];
    int isBackground;
} PROCESS;

/** @brief Estrutura do comando.
*
*          Esta estrutura guarda todas as informações
*          pertinentes de um comando.
*          Nesta implementação, um comando é definido
*          como um execultável ou builtin e seus argumentos.
*          cat arquivo | grep palavra | cut -c 1-6 tem 3 comandos,
*          por exemplo.
*
*/
typedef struct
{
    char **args;
    int isBackground;
    int input_r;
    int output_r;
    int output_r_append;
    int size;
    char * output_r_filename;
    char * input_r_filename;
    int id;
    int pipe[2];
    pid_t pid;
} COMMAND;

/** @brief Nó das listas de comandos e processos.
*
*          Temos na mesma estrutura de nó um ponteiro para comando
*          e outro para processo. Isso foi feito para facilitar a
*          implementação, os dois não devem ser usados juntos.
*
*/
typedef struct NODE
{
    PROCESS *proc;
    COMMAND *cmd;
    struct NODE *next;
    struct NODE *prev;
} NODE;


/** @brief Estrutura da lista em si.
*
*/
typedef struct
{
    NODE *first;
    NODE *last;
} LIST;


/** @brief Inicializa a lista para uso posterior.
*   @param list - Ponteiro para lista a ser inicializada.
*   @return void.
*/
void ListCreate(LIST *list);

/** @brief Inicializa um comando (estrutura) para
*          uso posterior.
*   @param cmd - Ponteiro para comando (estrutura)
*          a ser inicializada.
*   @return void.
*/
void initCommand(COMMAND *cmd);

/** @brief Verifica se uma lista está vazia.
*   @param list - Ponteiro para a lista.
*   @return Se a lista está vazia.
*   @retval 1 - Sim.
*   @retval 0 - Não.
*/
int ListIsEmpty(LIST *list);

/** @brief Insere um processo e/ou um comando na lista.
*
*          Na prática, deve ser usada uma lista para
*          comandos e outra para processos.
*          Elas só estão na mesma estrutura para evitar
*          repetição de código.
*          Caso esteja inserindo em uma lista de processos,
*          o argumento cmd deve ser NULL.
*          Caso esteja inserindo em uma lista de comandos,
*          o argumento proc deve ser NULL.
*   @param list - Ponteiro para a lista.
*   @param proc - Ponteiro para processo (estrutura).
*   @param cmd - Ponteiro para comando (estrutura).
*   @return Se foi possível inserir.
*   @retval 1 - Sim.
*   @retval 0 - Não.
*/
int ListInsert(LIST *list, PROCESS *proc, COMMAND *cmd);

/** @brief Remove um processo da lista pelo seu ID.
*   @param list - Ponteiro para a lista de processos.
*   @param pid - ID do processo.
*   @return Se a remoção foi feita.
*   @retval 1 - Sim.
*   @retval 0 - Não.
*/
int ListRemoveByPid(LIST *list, pid_t pid);

/** @brief Retorna o processo que está rodando em foreground.
*   @param list - Ponteiro para a lista de processos.
*   @return Processo em questão. (NULL caso não tenha um).
*/
PROCESS *ListGetCurrentProcess(LIST *list);

/** @brief Busca e retorna o processo parado mais recente.
*   @param list - Ponteiro para a lista de processos.
*   @return Processo em questão. (NULL caso não tenha um).
*/
PROCESS * ListGetLastStopped(LIST *list);

/** @brief Retorna um processo pelo seu id.
*   @param list - Ponteiro para a lista.
*   @param pid - Id do processo a ser procurado.
*   @return Ponteiro para o processo. (NULL caso
*           não encontrado).
*/
PROCESS * ListGetProcess(LIST *list, pid_t pid);

/** @brief Libera espaço alocado pela lista de comandos.
*   @param list - Ponteiro para a lista de comandos.
*   @return void.
*/
void ListPurgeCmds(LIST *list);

#endif
