/*************************************************************************
	> File Name: message.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月19日 星期一 19时31分39秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "./network.h"
#include "./public.h"
#include "./control.h"
#include <fcntl.h>

pthread_mutex_t mutex;
pthread_mutex_t mutex1;

void *network_recv(void *arg)
{
    int connfd = *(int *)arg;
    DataType data_recv;


    pthread_mutex_lock(&mutex);
    recv_safety(connfd, &data_recv);

    parse_task(&data_recv, connfd);
    pthread_mutex_unlock(&mutex);

    BodyData *data = data_recv.pcontent;
           
    if(data_recv.datahead.agreement_id == 0x04)
    {
        printf("CenterServer:tempre: %.2f",data->tempre);
        printf("  wind: %.2f",data->wind);
        printf("  duty: %.2d",data->duty);
        printf("  noise: %.2f\n\n", data->noise);
    }
    //printf("free malloc: %p\n",data_recv.pcontent); 
    free(data_recv.pcontent);
}

int add_fd(int epfd, int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    if(-1 == ret)
    {
        perror("epoll_ctl");
        return -1;
    }
    
    return 0;
}

int del_fd(int epfd, int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
    if(-1 == ret)
    {
        perror("epoll_ctl");
        return -1;
    }
    return 0;
}

int message_manger(int sockfd)
{
    struct sockaddr_in cli;
    int len = sizeof(cli);

    struct epoll_event rev[10];
    int epfd = epoll_create(10);
    if(-1 == epfd)
    {
        perror("epoll_create");
        return -3;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex1, NULL);
    
    add_fd(epfd, sockfd);

    while(1)
    {
        char buf[1024] = {0};
        int ep_ret = epoll_wait(epfd, rev, 10, -1);
        //printf("eo_ret = %d\n",ep_ret);

        int i = 0;
        for(i = 0; i < ep_ret; i++)
        {
            if(rev[i].data.fd == sockfd)
            {
                int conn = accept(sockfd, (struct sockaddr *)&cli, &len);
           
                //printf("connfd = %d\n",conn);
                if(-1 == conn)
                {
                    perror("messager_manger: fail accept");
                    continue;
                }
                add_fd(epfd, conn);
            }
            else
            {
                pthread_t net_recv_tid;
                //printf("rev[%d] = %d\n",i, rev[i].data.fd);
                pthread_create(&net_recv_tid, NULL, network_recv, (void *)&rev[i].data.fd);
                pthread_join(net_recv_tid, NULL);
            }
        }
        //pthread_t net_send_tid;
        //pthread_create(&net_recv_tid, NULL, send_data_for_each, (void *)&recv[i].data.fd);
    }
    close(sockfd); 
    return 0;
}
