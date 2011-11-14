#include "cmdQueue.h"

void queueCreate(CMD_QUEUE *queue)
{
    queue->last = 0;
    queue->first = 0;
}

int queueEmpty(CMD_QUEUE *queue)
{
    return (queue->first == queue->last);
}

int queueFull(CMD_QUEUE *queue)
{
    return (((queue->last+1) % SIZE) == queue->first);
}

int queueRemove(CMD_QUEUE *queue, CMD *item)
{
    if (!queueFull(queue))
    {
        queue->items[queue->last] = *item;
        queue->last = (queue->last+1) % SIZE;
        return 1;
    }
    return 0;
}
int queueAdd(CMD_QUEUE *queue, CMD *item)
{
    if (!queueEmpty(queue))
    {
        *item = queue->items[queue->first];
        queue->first = (queue->first+1) % SIZE;

        return 1;
    }
    return 0;
}
int queueCount(CMD_QUEUE *queue)
{
    if (queue->last >= queue->first)
    {
        return (queue->last - queue->first);
    }
    else
    {
        return (SIZE - (queue->first - queue->last));
    }
}
