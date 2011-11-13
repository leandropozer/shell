#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

void ListCreate(LIST *list)
{
    list->first = NULL;
    list->last = NULL;
}

int ListIsEmpty(LIST *list)
{
    if(list->first == NULL) return 1;
    return 0;
}


int ListInsert(LIST *list, ITEM *item)
{
    NODE *newNode = (NODE *)malloc(sizeof(NODE));

    if (newNode != NULL)
    {
        newNode->item = item;
        newNode->next = NULL;
        newNode->prev = list->last;

        if (list->last != NULL)
            list->last->next = newNode;
        else
            list->first = newNode;

        list->last = newNode;
        return 1;
    }
    return 0;
}

int ListGetByPid(LIST *list, pid_t pid, ITEM *item)
{
    NODE *aux = list->first;
    while (aux != NULL)
    {
        if (aux->item->pid == pid)
        {
            item = aux->item;
            return 1;
        }
        aux = aux->next;
    }

    return 0;
}

int ListGetRunningProcess(LIST *list, ITEM *item)
{
    NODE *aux = list->first;
    while (aux != NULL)
    {
        if(strcmp(aux->item->status, "Running") == 0)
        {
            strcpy(aux->item->status, "Stopped");
            item = aux->item;
            return 1;
        }
        aux = aux->next;
    }
    return 0;
}


int ListRemoveByPid(LIST *list, pid_t pid)
{
    if (!ListIsEmpty(list))
    {
        int i;
        NODE *aux = list->first;

        while(aux->item->pid != pid)
        {
            if (aux != list->last)
                aux = aux->next;
            else return 0;
        }

        if (aux == list->first)
            list->first = aux->next;
        else
            aux->prev->next = aux->next;

        if (aux == list->last)
            list->last = aux->prev;
        else
            aux->next->prev = aux->prev;

        free(aux->item);
        free(aux);

        return 1;
    }
    return 0;
}

pid_t ListLastStoppedToBg(LIST *list)
{
    if(!ListIsEmpty(list)) {
        NODE *aux = list->last;
        while (aux != NULL)
        {
            if(strcmp(aux->item->status, "Stopped") == 0)
            {
                aux->item->isBackground = 1;
                strcpy(aux->item->status, "Running");
                return aux->item->pid;
            }
            aux = aux->prev;
        }
    }
    return -1;
}

pid_t ListToBg(LIST *list, pid_t pid)
{
    if(!ListIsEmpty(list)) {
        NODE *aux = list->last;
        while (aux != NULL)
        {
            if(aux->item->pid == pid)
            {
                aux->item->isBackground = 1;
                strcpy(aux->item->status, "Running");
                return aux->item->pid;
            }
            aux = aux->prev;
        }
    }
    return -1;
}

pid_t ListLastToFg(LIST *list)
{
    if(!ListIsEmpty(list)) {
        NODE *aux = list->last;
        if (aux != NULL)
        {
            aux->item->isBackground = 0;
            strcpy(aux->item->status, "Running");
            return aux->item->pid;
        }
        aux = aux->prev;
    }
    return -1;
}

pid_t ListToFg(LIST *list, pid_t pid)
{
    if(!ListIsEmpty(list)) {
        NODE *aux = list->last;
        while (aux != NULL)
        {
            if(aux->item->pid == pid)
            {
                aux->item->isBackground = 0;
                strcpy(aux->item->status, "Running");
                return aux->item->pid;
            }
            aux = aux->prev;
        }
    }
    return -1;
}


