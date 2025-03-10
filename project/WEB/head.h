/*************************************************************************
	> File Name: head.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月05日 星期一 09时12分29秒
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct http_req
{
    char url[512];
    char method[128];
    char content[1024];
}HTTP_REQ_T;

#endif
