/*************************************************************************
	> File Name: network.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月18日 星期日 15时44分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "network.h"
#include "../CenterServer/public.h"
#include "./control.h"

void parse_task(DataType *data, int sockfd)
{
    switch(data->datahead.agreement_id)
    {
        case ZHU_CE_JIAN_QUAN:      // 注册鉴权id 0x01
            break;
        case XIN_TIAO_JIAN_CE:      // 心跳检测id 0x02 
            break;
        case XIAO_XI_ZHU_CE:        // 消息注册id 0x03
            get_message(*data, sockfd);
            break;
        case TONG_YONG_YING_DA:     // 通用应答id 0x04
            print_get_data(data);
            break;
        default:
            printf("Unknow protocol\n");
            break;
    }
    return ;
}

int send_safety(int sockfd, DataType *data)
{
    if(NULL == data)
    {
        return -1;
    }

    ssize_t size = send(sockfd, &(data->datahead), sizeof(packet_head), 0);
    if(size < 0)
    {
        perror("send_data: send_fail");
        return -1;
    }
    size = send(sockfd, data->pcontent, data->datahead.size, 0);
    if(size < 0)
    {
        perror("send_data: send_fail");
        return -1;
    }

    return 0;
}

int recv_safety(int sockfd, DataType *data)
{
    int headsize = 0;
    //headsize < 8
    while(headsize < sizeof(packet_head))
    {
        headsize += recv(sockfd, &(data->datahead)+headsize, sizeof(packet_head)-headsize, 0);
    }
        
    data->pcontent = malloc(data->datahead.size);

    //printf("recv_safety malloc: %p\n",data->pcontent);
    if(NULL == data->pcontent)
    {
        printf("recv_safety :fail malloc\n");
        return -1;
    }

    int size = 0;
    int sizeData = data->datahead.size;
    while(size < sizeData)
    {
        size += recv(sockfd, (data->pcontent), sizeData-size, 0);
        if(size < 0)
        {
            return -1;
        }
    }

    return size;
}

int socket_init_center(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("CenterServer: fail socket\n");
        return -1;
    }

    int val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in ser;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(LOCAL_PORT);
    ser.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int ret = bind(sockfd, (struct sockaddr *)&ser, sizeof(ser));
    if(-1 == ret)
    {
        perror("CenterServer: fail bind\n");
        return -1;
    }

    listen(sockfd, 6);
    
    return sockfd;
}



