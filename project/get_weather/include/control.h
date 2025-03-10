/*************************************************************************
	> File Name: control.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月30日 星期二 11时34分14秒
 ************************************************************************/

#ifndef _CONTROL_H
#define _CONTROL_H

extern char recv_buff[4096];
extern char json_buff[4096];

extern void screen_model_choose(void);
extern void screen_name_input(void);
extern int recv_http_response(int sockfd);
extern int send_http_Realtime(int sockfd,char *cityNM);
extern int send_http_future(int sockfd, char *cityNM);
extern int json_display(int input_num);

#endif
