/*************************************************************************
	> File Name: message.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月20日 星期二 09时51分31秒
 ************************************************************************/

#ifndef _MESSAGE_H
#define _MESSAGE_H


extern int add_fd(int epfd, int fd);
extern int del_fd(int epfd, int fd);
extern int message_manger(int sockfd);

#endif
