/*************************************************************************
	> File Name: register.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年09月03日 星期二 23时39分26秒
 ************************************************************************/

#include <stdio.h>
#include <sqlite3.h>
#include "register.h"

int callback2(void *arg, int column, char **value, char **title)
{
    printf("%s\n",value[0]);
}

int create_table_sqlite(void)
{
    sqlite3 *pdb = NULL;

    int ret = sqlite3_open("./usr_register.db", &pdb);
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
    char buff[256] = {0};

    sqlite3 *pdb = NULL;

    int ret = sqlite3_open("./usr_register.db", &pdb);
    if(SQLITE_OK != ret)
    {
        printf("sqlite3_open fail:%s\n",sqlite3_errmsg(pdb));
        return -1;
    }

    sprintf(buff, "select id from userdata where user='%s';", username);
    printf("%s\n",buff);

    int ret1 = sqlite3_exec(pdb, buff, callback2, NULL, NULL);
    if(ret1 != 4)
    {
        sprintf(buff,"insert into userdata values(NULL,\"%s\",\"%s\");",username, password);
        printf("%s\n",buff);

        ret = sqlite3_exec(pdb, buff, NULL, NULL, NULL);
        if(SQLITE_OK != ret)
        {
            printf("sqlite3_open fail:%s\n",sqlite3_errmsg(pdb));
            return -1;
        }
        sqlite3_close(pdb);

    }
    else
    {
        printf("该用户名已经被注册!\n");
        sqlite3_close(pdb);
        return 1;
    }
    
    sqlite3_close(pdb);

    return 0;
}


int find_user_data(const char *username, const char *password)    //查找数据库中是否存在对应的数据信息
{
    char buff[256] = {0};
    sqlite3 *pdb = NULL;

    int ret = sqlite3_open("usr_register.db", &pdb);
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

