/*************************************************************************
	> File Name: save_data.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年09月03日 星期二 20时36分32秒
 ************************************************************************/

#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <sqlite3.h>
#include <string.h>
#include "../CenterServer/public.h"
#include "save_data.h"

char cur_table[32] = {0};
int today = 0;

int init_tcp_ser(const char *ip, unsigned short port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("fail socket");
		return -1;
	}
	
	struct sockaddr_in ser;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(port);
	ser.sin_addr.s_addr = inet_addr(ip);
	int ret = bind(sockfd, (struct sockaddr *)&ser, sizeof(ser));
	if (-1 == ret)
	{
		perror("fail bind");
		return -1;
	}

	ret = listen(sockfd, 3);
	if (-1 == ret)
	{
		perror("fail listen");
		return -1;
	}

	return sockfd;

}

int print_ser_data(DataType data_recv)
{
    BodyData *data = (BodyData *)data_recv.pcontent;

    printf("ser_data: tempre: %.2f",data->tempre);
    printf("  wind: %.2f",data->wind);
    printf("  duty: %.2d",data->duty);
    printf("  noise: %.2f\n\n", data->noise);

    return 0;
}

/*
int prase_recv(char *recv_buff)
{
    int id = *(int *)recv_buff;
    DataType pdata;

    if(id == 0x04)
    {
        Packet_t data = *(Packet_t *)recv_buff;
        
        pdata.datahead = data.datahead;
        pdata.pcontent = &data.databody;
        print_ser_data(pdata);
        save_data(data);
    }

    return 0;
}
*/
int save_data(Packet_t data)
{
    sqlite3 *pdb = NULL;
    int ret = sqlite3_open("/home/linux/Desktop/project/IOT/server/sqlite3_save/ser_data", &pdb);
    if(ret != SQLITE_OK)
    {
        printf("fail sqlite3_open: %s\n", sqlite3_errmsg(pdb));
        return -1;
    }

    time_t tim = time(NULL);
    struct tm *ptime = localtime(&tim);

    if(ptime->tm_mday != today)
    {
        char temp[32] = {0};
        char table[256] = {0};
        
        strcpy(temp, cur_table);
        sprintf(cur_table,"DataTable%d_%d_%d",ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday);
        sprintf(table, "create table if not exists %s(id INTEGER PRIMARY KEY AUTOINCREMENT, time TEXT, temperture REAL, wind REAL, duty int, noise REAL);", cur_table);
        int ret = sqlite3_exec(pdb, table, NULL, NULL, NULL);
        if(ret != SQLITE_OK)
        {
            perror("save_data: fail sqlite3_exec");
            return -1;
        }
        
    }
        char buff[256] = {0};

        sprintf(buff,"insert into %s values(NULL, \"%d-%d-%d %d:%d:%d\", %f, %f, %d, %f);",cur_table, ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday, ptime->tm_hour, ptime->tm_min, ptime->tm_sec, data.databody.tempre, data.databody.wind, data.databody.duty, data.databody.noise);

        ret = sqlite3_exec(pdb, buff, NULL, NULL, NULL);
        if(ret != SQLITE_OK)
        {
            perror("save_data: fail sqlite3_exec");
            return -1;
        }
        today = ptime->tm_mday;

        sqlite3_close(pdb);
    
    return 0;
}

