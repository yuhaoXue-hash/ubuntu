/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月29日 星期一 17时21分18秒
 ************************************************************************/

#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "cJSON.h"
#include "control.h"
#include "logic.h"

char recv_buff[4096] = {0};
char json_buff[4096] = {0};

int init_clit_tcp(const char *ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("fail socket\n");
        return -1;
    }

    struct sockaddr_in cli;

    cli.sin_family = AF_INET;
    cli.sin_port = htons(port);
    cli.sin_addr.s_addr = inet_addr(ip);

    int ret = connect(sockfd, (struct sockaddr *)&cli, sizeof(cli));
    if(-1 == ret)
    {
        perror("fail connect\n");
        return 0;
    }
    
    return sockfd;
}

int get_num(void)
{
    int num = 0;
    scanf("%d",&num);
    if(num == 3)
    {
        logic_write(3, "用户退出程序");
        return num;
    }

    char *s = (char*)&num;
    logic_write(1, s);

    return num;
}

int do_control(int flag, int sockfd)
{
    char cityNM[16] ={0};
    
    screen_name_input();
    
    scanf("%s",cityNM);
    logic_write(2, cityNM);
    if(flag == 1)
    {
        send_http_Realtime(sockfd, cityNM);
    }
    else if(flag == 2)
    {
        send_http_future(sockfd, cityNM);
    }

    return 0;
}

int main()
{
    int ret = 0;
    logic_init(); 

    while(1)
    {
        int sockfd = init_clit_tcp("192.168.1.181",80);
        if(-1 == sockfd)
        {
            perror("fail init\n");
            return -1;
        }
        
        logic_create();

        memset(recv_buff, 0, sizeof(recv_buff));
        memset(json_buff, 0, sizeof(json_buff));
        screen_model_choose();
        ret = get_num();
        if(ret == 3)
        {
            break;
        }
        do_control(ret, sockfd);
        recv_http_response(sockfd);
        json_display(ret);
        close(sockfd);
    }


    return 0;
}
