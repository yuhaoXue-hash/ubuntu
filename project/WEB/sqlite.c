/*************************************************************************
	> File Name: sqlite.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月05日 星期一 12时53分38秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include <sys/socket.h>
#include "sqlite.h"
#include"sqlite3.h"
#include "head.h"


int callback(void *arg, int column, char **value, char **title)
{
    char buff[512] = {0};
    int connfd = *(int *)arg;

	sprintf(buff, "<a href=\"data[%s]\"> <img border=\"0\" src=\"./source/%s\" alt=\"HTML 教程\" width=\"200\" height=\"200\"> <p>%s </p> <p>%s</p></a></p>", value[0], value[19], value[3], value[16]);
    
    send(connfd, buff, strlen(buff), 0);
    printf("%s\n",buff);

	return 0;
}

int callback1(void *arg, int column, char **value, char **title)
{
    char buff1[310000] = {0};
    int connfd = *(int *)arg;

    sprintf(buff1,"<img src=\"./source/%s\" width=\"300\" alt=\"some_text\"><br> <p>%s</p> <p>%s</p>",value[19], value[3], value[18]);
    send(connfd, buff1, strlen(buff1), 0);
    
    return 0;
}

int create_table_sqlite(void)
{
    sqlite3 *pdb = NULL;

    int ret = sqlite3_open("./source/123.db", &pdb);
    if(SQLITE_OK != ret)
    {
        printf("sqlite3_open fail:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    char *psql1 = "create table if not exists userdata(id INTEGER PRIMARY KEY AUTOINCREMENT, user text, password text);";

    ret = sqlite3_exec(pdb, psql1, NULL, NULL, NULL);
    if(SQLITE_OK != ret)
    {
        printf("sqlite3_open fail:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }
        
    sqlite3_close(pdb);

    return 0;
}

int update_userdata(char *username, char *password)          //注册页面注册接口
{
    char buff[128] = {0};

    sqlite3 *pdb = NULL;

    int ret = sqlite3_open("./source/123.db", &pdb);
    if(SQLITE_OK != ret)
    {
        printf("sqlite3_open fail:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }
    
    sprintf(buff,"insert into userdata values(NULL,\"%s\",\"%s\");",username, password);
    printf("%s\n",buff);

    ret = sqlite3_exec(pdb, buff, NULL, NULL, NULL);
    if(SQLITE_OK != ret)
    {
        printf("sqlite3_open fail:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    sqlite3_close(pdb);

    return 0;
}

int callback2(void *arg, int column, char **value, char **title)
{
    printf("%s\n",value[0]);
}

int find_user_data(const char *username, const char *password)    //查找数据库中是否存在对应的数据信息
{
    char buff[256] = {0};
    sqlite3 *pdb = NULL;

    int ret = sqlite3_open("./source/123.db", &pdb);
    if(SQLITE_OK != ret)
    {
        printf("%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    sprintf(buff, "select id from userdata where user='%s' and password='%s';", username, password);
    printf("%s\n",buff);

    int ret1 = sqlite3_exec(pdb, buff, callback2, NULL, NULL);
    if(ret1 != 4)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int find_goods_sqlite(int connfd)
{
    char buff[64] = {0};
    sqlite3 *pdb = NULL;

    int ret = sqlite3_open("./source/123.db", &pdb);
    if(SQLITE_OK != ret)
    {
        printf("sqlite3_open fail:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    sprintf(buff, "select * from goods where goods_name like \"%%%s%%\";",goodsname);
    //printf("find sql = %s\n", buff);
    sqlite3_exec(pdb, buff, callback, &connfd, NULL);
    memset(goodsname, 0, sizeof(goodsname));

    sqlite3_close(pdb);

    return 0;
}

int find_detailed_data(char *id, int connfd)
{
    char buff[64] = {0};
    sqlite3 *pdb = NULL;

    int ret = sqlite3_open("./source/123.db", &pdb);
    if(SQLITE_OK != ret)
    {
        printf("sqlite3_open fail:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    sprintf(buff, "select * from goods where goods_id='%s'",id);
    //printf("find sql = %s\n", buff);
    sqlite3_exec(pdb, buff, callback1, &connfd, NULL);
    memset(goodsname, 0, sizeof(goodsname));

    sqlite3_close(pdb);
    
}

