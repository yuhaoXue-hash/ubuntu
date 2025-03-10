/*************************************************************************
	> File Name: main_utf.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月15日 星期四 15时56分44秒
 ************************************************************************/

#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include "../CenterServer/public.h"
#include "../CenterServer/network.h"
#include "../send_data/ser.h"
#include"utf.h"

int print_display_data(DataType data_recv)
{
    BodyData *data = (BodyData *)data_recv.pcontent;

    printf("display_data: tempre: %.2f",data->tempre);
    printf("  wind: %.2f",data->wind);
    printf("  duty: %.2d",data->duty);
    printf("  noise: %.2f\n\n", data->noise);

    return 0;
}

int sockfd_init_display(void)
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


int display_screen(void)
{
    UTF8_INFO utf8_info;
    
    init_fb("/dev/fb0");
    init_UTF8_INFO(&utf8_info, 32, 32);

    draw_clear(0x00ffffff);

    display_show_utf8_str(&utf8_info, 0, 0, "大家好", 0x00ff0000, 0x00ffffff);

    uninit_utf8(&utf8_info);
}

void *recv_data(void *arg)
{
    int connfd = *(int *)arg;
    DataType data_recv;
    Packet_t data;
   
    while(1)
    {
        recv_safety(connfd, &data_recv);
       
        data = *(Packet_t *)&data_recv;
        print_display_data(data_recv);

        //display_screen();

        free(data_recv.pcontent);
    } 
}

int Message_registration_display_data(int sockfd ,const char *path)
{
    MSG_pack data;
    DataType send_msg;

    send_msg.datahead.agreement_id = 0x03;          //消息注册id 0x03
    send_msg.datahead.size = sizeof(MSG_pack);
    
    data.id = 3;
    data.sockfd = sockfd;
    data.pid = getpid();
    //printf("send_data : %d\n",getpid());
    strcpy(data.path, path);

    send_msg.pcontent = &data;

    send_safety(sockfd, &send_msg);

    return 0;
}

int main(int argc, char *argv[])
{
    char path[32] = {0};
    strcpy(path, argv[0]);
    printf("path:%s\n",path);

    int sockfd = sockfd_init_display();
    if(sockfd < 0)
    {
        perror("display data: sockfd_init");
        return -1;
    }

    Message_registration_display_data(sockfd, path);

    pthread_t tid;
    pthread_create(&tid, NULL, recv_data, (void *)&sockfd);

    pthread_join(tid, NULL);


    return 0;
}
