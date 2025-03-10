/*************************************************************************
	> File Name: queue.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月19日 星期一 14时19分03秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue_t *create_queue(void)
{
    Queue_t *queue = malloc(sizeof(Queue_t));
    if(NULL == queue)
    {
        perror("fail malloc\n");
        return NULL;
    }

    queue->pfront = NULL;
    queue->prear = NULL;
    pthread_mutex_init(&(queue->mutex), NULL);
    queue->clen = 0;

    return queue;
}

int push_queue(Queue_t *queue, Packet_t data)
{
    if(NULL == queue)
        return -1;

    QueNode_t *pNode = malloc(sizeof(QueNode_t));
    if(NULL == pNode)
    {
        perror("Queue:fail malloc");
        return -1;
    }

    pNode->data = data;
    pNode->pnext = NULL;

    if(queue->prear == NULL)
    {
        queue->pfront = pNode;
        queue->prear = pNode;
    }
    else
    {
        queue->prear->pnext = pNode;
        queue->prear = pNode;
    }

    return 0;
}

int pop_queue(Queue_t *queue, Packet_t *data)
{
    if(NULL == queue)
        return -1;

    if(queue->pfront == NULL)
    {
        printf("该队列为空\n");
        return -1;
    }

    QueNode_t *pNode = queue->pfront;

    if(data != NULL)
    {
        *data = pNode->data;
    }

    queue->pfront = pNode->pnext;
    if(queue->pfront == NULL)
    {
        queue->prear = NULL;
    }
    free(pNode);

    return 0;
}

void destroy_queue(Queue_t *queue)
{
    if(queue == NULL)
        return ;

    while(queue->pfront == NULL)
    {
        pop_queue(queue, NULL);
    }
    free(queue);
}



