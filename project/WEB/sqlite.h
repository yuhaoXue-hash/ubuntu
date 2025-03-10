/*************************************************************************
	> File Name: sqlite.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月05日 星期一 14时35分06秒
 ************************************************************************/

#ifndef _SQLITE_H
#define _SQLITE_H



extern char username[32];
extern char password[32];
extern char goodsname[32];

extern int create_table_sqlite(void);
extern int find_goods_sqlite(int connfd);
extern int find_detailed_data(char *id, int connfd);
extern int update_userdata(char *username, char *password);
extern int find_user_data(const char *username, const char *password);

#endif
