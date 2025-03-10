/*************************************************************************
	> File Name: queue.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月19日 星期一 14时19分20秒
 ************************************************************************/

#ifndef _QUEUE_H
#define _QUEUE_H

#include "public.h"

extern Queue_t *create_queue(void);
extern int push_queue(Queue_t *queue,Packet_t data);
extern int pop_queue(Queue_t *queue, Packet_t *data);
extern void destroy_queue(Queue_t *queue);


#endif

