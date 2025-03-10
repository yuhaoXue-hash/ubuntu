/*************************************************************************
	> File Name: ser.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月20日 星期二 17时22分22秒
 ************************************************************************/

#ifndef _SER_H
#define _SER_H

extern int sockfd_init_ser(const char *ip, int port);
extern  int sockfd_init_local(void);
extern int Message_registration_send_data(int sockfd ,const char *path);
extern int print_send_data(DataType data_recv);


#endif
