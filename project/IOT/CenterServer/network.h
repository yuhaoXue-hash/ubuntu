/*************************************************************************
	> File Name: network.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月18日 星期日 17时01分17秒
 ************************************************************************/

#ifndef _NETWORK_H
#define _NETWORK_H

#include "./public.h"

#define     ZHU_CE_JIAN_QUAN        0x01
#define     XIN_TIAO_JIAN_CE        0x02
#define     XIAO_XI_ZHU_CE          0x03
#define     TONG_YONG_YING_DA       0x04

extern int socket_init_center(void);
extern int send_safety(int sockfd, DataType *data);
extern int recv_safety(int sockfd, DataType *data);
extern void parse_task(DataType* data, int sockfd);

#endif
