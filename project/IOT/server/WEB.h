/*************************************************************************
	> File Name: WEB.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年09月03日 星期二 20时54分28秒
 ************************************************************************/

#ifndef _WEB_H
#define _WEB_H


typedef struct http_req
{
    char url[512];
    char method[128];
    char content[1024];
}HTTP_REQ_T;

extern int init_tcp_web(const char *ip, unsigned short port);
extern int send_http_head(int connfd);
extern int request_prase(char *preq_buff, HTTP_REQ_T *preq);
extern int respond_send_content(const char *pathname, int connfd);
extern int send_respond(int connfd, HTTP_REQ_T preq);
extern int content_prase(HTTP_REQ_T preq);

#endif
