/*************************************************************************
	> File Name: control.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月21日 星期三 15时37分20秒
 ************************************************************************/

#ifndef _CONTROL_H
#define _CONTROL_H

#include "./public.h"

MSG_pack MsgTable[5];

extern int print_message_table(MSG_pack MsgTable);
extern int get_message(DataType data, int sockfd);
extern int print_get_data(DataType *data_recv);

#endif
