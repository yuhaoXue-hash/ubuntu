/*************************************************************************
	> File Name: main.c

    > CenterServer

	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月18日 星期日 15时44分28秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include "../CenterServer/network.h"
#include "../CenterServer/public.h"
#include "../CenterServer/message.h"
#include "../CenterServer/process.h"

int main(int argc, const char *argv[])
{
    int sockfd = socket_init_center(LLOOP, LOCAL_PORT);
    if(-1 == sockfd)
    {
        perror("CenterServer: fail socket_init_ser");
        return -1;
    }

    printf("sockfd = %d\n",sockfd);
    struct sockaddr_in cli;
    socklen_t socklen = sizeof(cli);

    process_init("./CENTERSEVER_PARAM");
    message_manger(sockfd);


    return 0;
}
