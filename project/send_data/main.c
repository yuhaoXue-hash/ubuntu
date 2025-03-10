/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月20日 星期二 17时22分05秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>       
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "../CenterServer/queue.h"
#include "../CenterServer/network.h"
#include "../CenterServer/public.h"

int main(int argc, char *argv[])
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
    ser.sin_addr.s_addr = inet_addr("192.168.199.128");

    int ret = connect(sockfd, (struct sockaddr *)&ser, sizeof(ser));
    if(-1 == ret)
    {
        perror("fail connect");
        return -1;
    }

    return 0;
}
