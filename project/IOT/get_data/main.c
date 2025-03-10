/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月19日 星期一 16时07分24秒
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "../CenterServer/public.h"
#include "../CenterServer/queue.h"
#include "../CenterServer/network.h"

pthread_mutex_t mutex;
Queue_t *queue;

int socket_init_cli()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("fail socket");
        return -1;
    }

    struct sockaddr_in ser;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(LOCAL_PORT);
    ser.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int ret = connect(sockfd, (struct sockaddr *)&ser, sizeof(ser));
    if(-1 == ret)
    {
        perror("fail connect");
        return -1;
    }

    return sockfd;
}

int get_data(Packet_t *data)
{
    srand((unsigned int)time(NULL));

    data->datahead.agreement_id = 0x04;

    data->databody.tempre = (rand()%10000)/100.0;
    data->databody.wind = (rand()%100)/10.0;
    data->databody.duty = rand()%6000;
    data->databody.noise = (rand()%1000)/10.0;
    
    data->datahead.size = sizeof(data->databody);//这里固定16，不变，应该没问题

    return 0;
}

int printf_data(Packet_t data)
{
    printf("============================================\n");
    printf("get_data :agreement_id: %d  ", data.datahead.agreement_id);

    printf("tempre: %.2f  ",data.databody.tempre);
    printf("wind: %.2f  ",data.databody.wind);
    printf("duty: %.2d  ",data.databody.duty);
    printf("noise: %.2f\n\n", data.databody.noise);

    return 0;
}


void *get_data_thread(void *arg)
{
    Packet_t data;
   
    while(1)
    {
        get_data(&data);
     
        pthread_mutex_lock(&mutex);
        push_queue(queue, data);
        pthread_mutex_unlock(&mutex);

        sleep(2);
    }
}

void *send_data_thread(void *arg)
{
    int sockfd = *(int *)arg;
    Packet_t data;
         
    
    while(1)
    {
        if(queue->pfront != NULL)
        {
            pthread_mutex_lock(&mutex);
            pop_queue(queue, &data);
            pthread_mutex_unlock(&mutex);
            
            DataType data_send;

            data_send.datahead = data.datahead;
            data_send.pcontent = &(data.databody);

            printf_data(data);
            send_safety(sockfd, &data_send);
        }
        sleep(1);
    }
}


//**********消息注册初始化****************//
int Message_registration_get_data(int sockfd ,const char *path)
{
    MSG_pack data;
    DataType send_msg;

    send_msg.datahead.agreement_id = 0x03;          //消息注册id 0x03
    send_msg.datahead.size = sizeof(MSG_pack);
    
    data.id = 0;
    data.sockfd = sockfd;
    data.pid = getpid();
    //printf("send_data : %d\n",getpid());
    strcpy(data.path, path);

    send_msg.pcontent = &data;

    send_safety(sockfd, &send_msg);


    return 0;
}



int main(int argc, const char *argv[])
{
    char path[32] = {0};
    printf("%s\n",(char *)argv[0]);
    strcpy(path, argv[0]);

    pthread_mutex_init(&mutex, NULL);
    
    int sockfd = socket_init_cli();
    printf("socket = %d\n",sockfd);
    if(-1 == sockfd)
    {
        perror("fail socket_init_cli");
        return -1;
    }

    //printf("hello wrld\n");
    Message_registration_get_data(sockfd, path);
    //printf("hello wrld\n");

    queue = create_queue();
    if(NULL == queue)
    {
        perror("get_data: fail create_queue");
        return -1;
    }
    
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, get_data_thread, NULL);
    pthread_create(&tid2, NULL, send_data_thread, (void *)&sockfd);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;    
}


