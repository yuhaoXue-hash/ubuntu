/*************************************************************************
	> File Name: register.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年09月03日 星期二 23时41分42秒
 ************************************************************************/

#ifndef _REGISTER_H
#define _REGISTER_H

extern int create_table_sqlite(void);
extern int update_userdata(char *username, char *password);
extern int find_user_data(const char *username, const char *password);

#endif
