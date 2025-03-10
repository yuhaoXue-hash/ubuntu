/*************************************************************************
	> File Name: ser.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月20日 星期二 17时22分18秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "../CenterServer/network.h"
#include "../CenterServer/public.h"
#include "../CenterServer/control.h"

int sockfd_init_ser(const char *ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("fail socket");
        return -1;
    }

    struct sockaddr_in ser;
    socklen_t socklen = sizeof(ser);

    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = inet_addr(ip);


    int ret = connect(sockfd, (struct sockaddr *)&ser, socklen);
    if(-1 == ret)
    {
        perror("fail connect");
        return -1;
    }

    return sockfd;
}

int sockfd_init_local(void)
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


int Message_registration_send_data(int sockfd ,const char *path)
{
    MSG_pack data;
    DataType send_msg;

    send_msg.datahead.agreement_id = 0x03;          //消息注册id 0x03
    send_msg.datahead.size = sizeof(MSG_pack);
    
    data.id = 1;
    data.sockfd = sockfd;
    data.pid = getpid();
    //printf("send_data : %d\n",getpid());
    strcpy(data.path, path);

    send_msg.pcontent = &data;

    send_safety(sockfd, &send_msg);

    return 0;
}

int print_send_data(DataType data_recv)
{
    BodyData *data = (BodyData *)data_recv.pcontent;

    printf("send_data: tempre: %.2f",data->tempre);
    printf("  wind: %.2f",data->wind);
    printf("  duty: %.2d",data->duty);
    printf("  noise: %.2f\n\n", data->noise);

    return 0;
}

