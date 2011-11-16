#ifndef _LISTA_H_
#define _LISTA_H_
typedef struct
{
    pid_t pid;
    char command[256];
    char status[100];
    int isBackground;
} PROCESS;

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

typedef struct NODE
{
    PROCESS *proc;
    COMMAND *cmd;
    struct NODE *next;
    struct NODE *prev;
} NODE;

typedef struct
{
    NODE *first;
    NODE *last;
} LIST;


/** @brief Inicializa a lista para uso posterior.
*   @param list - Ponteiro para lista a ser inicializada.
*   @return Void.
*/
void ListCreate(LIST *list);
/** @brief Inicializa um comando (estrutura) para
*          uso posterior.
*   @param list - Ponteiro para comando (estrutura)
*          a ser inicializada.
*   @return Void.
*/
void initCommand(COMMAND *cmd);
/** @brief Verifica se uma lista está vazia.
*   @param list - Ponteiro para a lista.
*   @return Se a lista está vazia
*           Caso contrário.
*   @retval 1 - Sim.
*   @retval 0 - Não.
*/
int ListIsEmpty(LIST *list);
/** @brief Insere um processo e/ou um comando na lista.
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
*   @return Se foi possível inserir
*   @retval 1 - Sim.
*   @retval 0 - Não.
*/
int ListInsert(LIST *list, PROCESS *proc, COMMAND *cmd);
int ListRemoveByPid(LIST *list, pid_t pid);
PROCESS *ListGetCurrentProcess(LIST *list);

PROCESS * ListGetLastStopped(LIST *list);
/** @brief Retorna um processo (estrutura) pelo seu id.
*   @param list - Ponteiro para a lista.
*   @param pid - Id do processo a ser procurado.
*   @param proc - Ponteiro que apontará para lista
*   @return Se a lista está vazia
*           Caso contrário.
*   @retval 1 - Sim.
*   @retval 0 - Não.
*/
PROCESS * ListGetProcess(LIST *list, pid_t n);
void ListPurgeCmds(LIST *list);
LIST * childs;
#endif
