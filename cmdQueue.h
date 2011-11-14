#define SIZE 100

typedef struct
{
    int isBackground;
    int input_r;
    int output_r;
    int output_r_append;
} CMD;

typedef struct
{
    CMD items[SIZE];
    int last;
    int first;
} CMD_QUEUE;
