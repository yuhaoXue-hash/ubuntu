/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月30日 星期五 14时33分54秒
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <sqlite3.h>
#include "../CenterServer/public.h"
#include "save_data.h"
#include "WEB.h"
#include "register.h"

#define EPOLL_FD_MAX 1024

int sockser;
HTTP_REQ_T preq;
int fds1 = 0;
Packet_t data;

int epoll_fd_add(int fds, int fd, uint32_t event)
{
	struct epoll_event ev;
	ev.events = event;
	ev.data.fd = fd;
	int ret = epoll_ctl(fds, EPOLL_CTL_ADD, fd, &ev);
	if (ret < 0)
	{
		perror("fail epoll_ctl add");
		return -1;
	}

	return 0;
}

int epoll_fd_del(int fds, int fd)
{
	int ret = epoll_ctl(fds, EPOLL_CTL_DEL, fd, NULL);
	if (ret < 0)
	{
		//perror("fail epoll_ctl del");
		return -1;
	}

	return 0;
}

void *WEB_recv(void *arg)
{
    int fd = *(int *)arg;
    char buff[2048] = {0};
	
    ssize_t size = recv(fd, buff, sizeof(buff), 0);
	if (size < 0)
	{
		//perror("fail recv");
		epoll_fd_del(fds1, fd);
		close(fd);
		return NULL;
	}
	else if (0 == size)
	{
		epoll_fd_del(fds1, fd);
		close(fd);
		//printf("cli offline\n");
		return NULL;
	}
        request_prase(buff, &preq);   //解析报文中的url,method,content
        send_respond(fd, preq);       //根据url中的发送文件
}

void *ser_recv(void *arg)
{
    int fd = *(int *)arg;
	struct sockaddr_in cli;
	socklen_t clilen = sizeof(cli);


    while(1)
    {
        char recv_buff[128] = {0};
	
        ssize_t size = recv(fd, recv_buff, sizeof(recv_buff), 0);
        printf("szie = %d\n",size);
        if(size < -1 || size == 0)
        {
            printf("sever: recv fail\n");
            fd = accept(sockser, (struct sockaddr *)&cli, &clilen);
        }

        int id = *(int *)recv_buff;
        DataType pdata;

        printf("id == %d\n",id);
        if(id == 0x04)
        {
            data = *(Packet_t *)recv_buff;
        
            pdata.datahead = data.datahead;
            pdata.pcontent = &data.databody;
            print_ser_data(pdata);
            save_data(data);
            send(fd, recv_buff, sizeof(Packet_t), 0);   // 回复arm端
        }


    }

}

int main(int argc, const char *argv[])
{
	char buff[1024] = {0};
	struct sockaddr_in cli;
	socklen_t clilen = sizeof(cli);


    int create_ret = create_table_sqlite();
    if(create_ret == -1)
    {
        perror("create_table_sqlite fail\n");
        return -1;
    }

        sockser = init_tcp_ser(SER_ADDR, SER_PORT);
	    if (-1 == sockser)
	    {
		    return -1;
	    }

       
        printf("1. hello world\n");
        int connfd = accept(sockser, (struct sockaddr *)&cli, &clilen);
        if (connfd < 0)
        {
            perror("fail accept");
		    return -1;
	    }
        printf("hello world\n");

        pthread_t tid1;
        pthread_create(&tid1, NULL, ser_recv, (void*)&connfd);
	    
        int sockfd = init_tcp_web(SER_ADDR, WEB_PORT);
	    if (-1 == sockfd)
	    {
		    return -1;
	    }

	    fds1 = epoll_create(EPOLL_FD_MAX);
    	if (fds1 < 0)
    	{
	    	perror("fail epoll_create");
	    	return -1;
    	}
	
    	epoll_fd_add(fds1, sockfd, EPOLLIN);
	
	    struct epoll_event evs[EPOLL_FD_MAX];


	    while (1)
	    {
		 
	        struct sockaddr_in cli;
	        socklen_t clilen = sizeof(cli);

            int cnt = epoll_wait(fds1, evs, EPOLL_FD_MAX, -1);
		    if (cnt < 0)
		    {
			    perror("fail epoll_wait");
			    return -1;
		    }
		    for (int i = 0; i < cnt; i++)
		    {
			    if (sockfd == evs[i].data.fd)
			    {
				    int connfd = accept(sockfd, (struct sockaddr *)&cli, &clilen);
				    if (connfd < 0)
				    {
					    perror("fail accept");
					    continue;
				    }
				    epoll_fd_add(fds1, connfd, EPOLLIN);
				    printf("[%s : %d] getline\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
			    }
			    else
			    {
                    pthread_t tid;
                    pthread_create(&tid, NULL, WEB_recv, (void *)&evs[i].data.fd);
			    }
		    }

	    }
    
	return 0;
}
