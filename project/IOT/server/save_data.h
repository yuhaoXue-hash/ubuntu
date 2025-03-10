/*************************************************************************
	> File Name: save_data.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年09月03日 星期二 20时41分10秒
 ************************************************************************/

#ifndef _SAVE_DATA_H
#define _SAVE_DATA_H

extern char cur_table[32];
extern int today;

extern int init_tcp_ser(const char *ip, unsigned short port);
//extern int prase_recv(char *recv_buff);
extern int save_data(Packet_t data);
extern int print_ser_data(DataType data_recv);

#endif
