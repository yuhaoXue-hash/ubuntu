/*************************************************************************
	> File Name: public.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月19日 星期一 14时24分24秒
 ************************************************************************/

#ifndef _PUBLIC_H
#define _PUBLIC_H

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#define LLOOP    INADDR_LOOPBACK //本地回环
#define LOCAL_PORT  50000         //本地端口号

#define  SER_ADDR    "192.168.1.181"
#define  SER_PORT    50001
#define  WEB_PORT    8080

//******数据身体部分*******//
typedef struct bodydata
{
    float tempre;
    float wind;
    int duty;
    float noise;
}BodyData;

///******数据帧头帧************//
typedef struct head
{
    int agreement_id;
    int size;
}packet_head;

//*******采集数据包***********//
typedef struct pack_get_date
{
    packet_head datahead;
    BodyData databody;
}Packet_t;

//*******打包数据帧***********//
typedef struct packet
{
    packet_head datahead;
    void *pcontent;
}DataType;

//******队列节点***********//
typedef struct quenode
{
    Packet_t data;
    struct quenode *pnext;
}QueNode_t;

//*******队列头节点***********//
typedef struct queue
{
    QueNode_t *pfront;
    QueNode_t *prear;
    pthread_mutex_t mutex;
    int clen;
}Queue_t;


//*******注册消息****************//
typedef struct msg_reg
{
    int id;
    int sockfd;
    pid_t pid;
    char path[32];
}MSG_pack;

typedef struct msg
{
    packet_head datahead;
    MSG_pack msg;
}MSG_t;



#endif

