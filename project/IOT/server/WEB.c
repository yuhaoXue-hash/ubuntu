/*************************************************************************
	> File Name: WEB.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年09月03日 星期二 20时31分43秒
 ************************************************************************/

#include<stdio.h>
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
#include "WEB.h"
#include "../CenterServer/public.h"
#include "register.h"

extern Packet_t data;

char username[32] = {0};
char password[32] = {0};

const char* htmlContent = "<!DOCTYPE html>\
<html>\
<head>\
<meta charset=\"utf-8\">\
<title>MY HTML</title>\
</head>\
<body>\
<style>\
    body {\
        background-image: url('./image/datadisplay.png');\
        background-size: cover;\
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; /* 更现代的字体 */\
        color: #333; /* 设置文本颜色 */\
        margin: 0;\
        padding: 0;\
        height: 100%;\
    }\
    .container {\
        max-width: 800px; /* 调整容器最大宽度 */\
        margin: 50px auto; /* 垂直和水平居中 */\
        padding: 30px; /* 内边距 */\
        background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%); /* 渐变背景 */\
        border-radius: 15px; /* 圆角边框 */\
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); /* 阴影效果 */\
        transition: transform 0.3s ease-in-out; /* 鼠标悬停时的平滑过渡效果 */\
    }\
    .container:hover {\
        transform: translateY(-5px); /* 鼠标悬停时上移 */\
    }\
    h1 {\
        text-align: center; /* 标题居中 */\
        color: #007BFF; /* 标题颜色 */\
        margin-bottom: 30px; /* 标题下边距 */\
    }\
    .data-display {\
        font-size: 20px; /* 字体大小 */\
        color: #333; /* 数据文本颜色 */\
    }\
    .data-display p {\
        display: flex; /* 使用Flex布局 */\
        align-items: center; /* 垂直居中 */\
        justify-content: space-between; /* 分散对齐 */\
        margin-bottom: 15px; /* 段落间距 */\
        padding: 10px; /* 段落内边距 */\
        border: 1px solid #e1e1e1; /* 边框 */\
        border-radius: 8px; /* 圆角边框 */\
        background-color: #ffffff; /* 背景颜色 */\
        box-shadow: 0 2px 4px rgba(0, 0, 0, 0.05); /* 轻微的阴影效果 */\
    }\
    .data-label {\
        font-weight: bold; /* 标签加粗 */\
        flex: 1; /* 标签占据剩余空间 */\
        text-align: left; /* 文本左对齐 */\
    }\
    .data-value {\
        flex: 2; /* 数据值占据更多空间 */\
        text-align: right; /* 文本右对齐 */\
        color: #FF5733; /* 数据值颜色 */\
    }\
</style>\
</head>\
<body>\
<a href=\"./home.html\">\
    <img border=\"0\" src=\"./image/shouye.png\" alt=\"HTML 教程\" width=\"50\" height=\"50\">\
</a>\
<div class=\"container\">\
    <center>\
        <h1>智能塔吊检测系统</h1>\
        <div class=\"data-display\">\
            <p><span class=\"data-label\">温度:</span> <span id=\"temperature\" class=\"data-value\">加载中...</span>°C</p>\
            <p><span class=\"data-label\">风力:</span> <span id=\"wind\" class=\"data-value\">加载中...</span>级</p>\
            <p><span class=\"data-label\">扬尘:</span> <span id=\"dust\" class=\"data-value\">加载中...</span>μg/m³</p>\
            <p><span class=\"data-label\">噪声:</span> <span id=\"noise\" class=\"data-value\">加载中...</span>dB</p>\
        </div>\
    </center>\
</div>\
\
<script>";


int init_tcp_web(const char *ip, unsigned short port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("fail socket");
		return -1;
	}
    
    int val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	
    struct sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(port);
	ser.sin_addr.s_addr = inet_addr(ip);
	int ret = bind(sockfd, (struct sockaddr *)&ser, sizeof(ser));
	if (-1 == ret)
	{
		perror("fail bind");
		return -1;
	}

	ret = listen(sockfd, 100);
	if (-1 == ret)
	{
		perror("fail listen");
		return -1;
	}

	return sockfd;
}

int send_http_head(int connfd)
{
    char *phead = "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html;charset=UTF-8\r\n"
        "Connection: close\r\n"
        "Server: WEB_SERVER\r\n\r\n";

    printf("%s",phead);
    ssize_t size = send(connfd, phead, strlen(phead), 0);
    if(size == -1)
    {
        perror("fail send\n");
        return -1;
    }

    return 0;
}

int respond_send_content(const char *pathname, int connfd)
{
    char buff[1024] = {0};

    printf("send filename = %s\n", pathname);
    int fd = open(pathname, O_RDONLY);
    if(-1 == fd)
    {
        perror("fail open\n");
        return -1;
    }
    
    while(1)
    {
        ssize_t size = read(fd, buff, sizeof(buff));
        if(0 == size)
        {
            break;
        }
        
        int ret = send(connfd, buff, size, 0);
        if(-1 == ret)
        {
            perror("fail send\n");
            return -1;
        }
    }
    close(fd);

    return 0;
}

int request_prase(char *preq_buff, HTTP_REQ_T *preq)
{
    char buff[2048] = {0};
    
    strcpy(buff, preq_buff);

    char *req_method = strtok(buff, " ");
    if(req_method == NULL)
    {
        perror("fail strtok\n");
        return -1;
    }
    strcpy(preq->method, req_method);

    char *req_url = strtok(NULL, " ");
    if(req_url == NULL)
    {
        perror("fail strtok\n");
        return -1;
    }
    strcpy(preq->url, req_url);


    char *req_content = strstr(preq_buff, "\r\n\r\n");
    if(req_content != NULL)
    {
        req_content += 4;
    }
    else
    {
        perror("fail strstr\n");
        return -1;
    }
    strcpy(preq->content, req_content);

    printf("method :%s\n",preq->method);
    printf("url :%s\n",preq->url);
    printf("content :%s\n",preq->content);

    return 0;
}

int content_prase(HTTP_REQ_T preq)
{
    int i = 0;
    //printf("%s\n",preq.content);
    char *pUse = strstr(preq.content, "username=");
    char *pPase =  strstr(preq.content, "passname=");

    if(pUse != NULL && pPase != NULL)
    {
        pUse += 9;
        pPase += 9;
        while(*pUse != '&')
        {
            username[i] = *pUse;
            i++;
            pUse++;
        }
        strcpy(password, pPase);

        printf("username:%s\n",username);
        printf("passname:%s\n",password);
    }

    return 0;
}

int send_data_display(int fd)
{
    char buff[1024] = {0};
    printf("%s\n",htmlContent);
    send(fd, htmlContent, strlen(htmlContent), 0);

    sprintf(buff, "document.getElementById('temperature').textContent = '%f'; document.getElementById('wind').textContent = '%f'; document.getElementById('dust').textContent = '%d';document.getElementById('noise').textContent = '%f';</script></body></html>",data.databody.tempre,data.databody.wind,data.databody.duty,data.databody.noise);

    send(fd, buff, strlen(buff), 0);
    printf("%s\n",buff);
}

int send_respond(int connfd, HTTP_REQ_T preq)
{
    char buff[1024] = {0};

    send_http_head(connfd);
    
    if(strcmp("GET", preq.method) == 0)
    {
        if((strcmp("/", preq.url) == 0) || (strcmp("/home.html", preq.url) == 0))
        {
            respond_send_content("./html/home.html", connfd);
        }
        else if(strstr(preq.url, ".png") != NULL || NULL != strstr(preq.url, ".gif") || strstr(preq.url, ".jpg") != NULL)
        {
            sprintf(buff, "./html%s", preq.url);
            printf("%s\n",buff);
            respond_send_content(buff, connfd);
        }
    }
    else if(strcmp("POST", preq.method) == 0)
    {           
        if(strcmp(preq.url, "/register.html") == 0)
        {
            sprintf(buff, "./html%s",preq.url);
            respond_send_content(buff, connfd);
        }
        else if(strstr(preq.url, ".html") != NULL)
        {
            printf("preq.url: %s\n",preq.url);
            if(strcmp(preq.url,"/home.html") == 0)
            {
                content_prase(preq);
                update_userdata(username,password);   
                memset(username, 0, sizeof(username));
                memset(password, 0, sizeof(password));
                sprintf(buff, "./html%s",preq.url);
                respond_send_content(buff, connfd);
            }
            else if(strcmp(preq.url, "/datadisplay.html") == 0)
            {
                content_prase(preq);
                if(find_user_data(username, password) == 1)
                {
                    memset(username, 0, sizeof(username));
                    memset(password, 0, sizeof(password));
                     
                    send_data_display(connfd);
                }
                else
                {
                    respond_send_content("./html/home.html", connfd);
                }
            }
        }
    }
}

