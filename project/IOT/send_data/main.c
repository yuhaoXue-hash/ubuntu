/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月23日 星期五 19时22分10秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "../CenterServer/network.h"
#include "../CenterServer/control.h"
#include "../CenterServer/public.h"
#include "../CenterServer/queue.h"
#include "ser.h"


sqlite3 *pdb = NULL;
Queue_t *queue = NULL;
pthread_mutex_t mutex;

void *recv_data_thread(void *arg)
{
    int connfd = *(int *)arg;
    DataType data_recv;
    Packet_t data;
   
    while(1)
    {
        recv_safety(connfd, &data_recv);
       
        data = *(Packet_t *)&data_recv;
        print_send_data(data_recv);


        data.datahead = data_recv.datahead;
        data.databody = *(BodyData *)(data_recv.pcontent);

        pthread_mutex_lock(&mutex);
        push_queue(queue, data);
        pthread_mutex_unlock(&mutex);  

        free(data_recv.pcontent);
    }
}

void *send_data_thread(void *arg)
{
    int connfd = *(int *)arg;
    Packet_t data;

    while(1)
    {
        if(queue->pfront != NULL)
        {
            time_t tim = time(NULL);
            struct tm *ptime = localtime(&tim);
            
            pthread_mutex_lock(&mutex);
            pop_queue(queue, &data);
            pthread_mutex_unlock(&mutex);
            
            DataType data_send;
        
            data_send.datahead = data.datahead;
            data_send.pcontent = &(data.databody);

            print_send_data(data_send);

            char buff[256] = {0};

            sprintf(buff,"insert into send_save values(NULL, \"%d-%d-%d %d:%d:%d\", %f, %f, %d, %f);", ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday, ptime->tm_hour, ptime->tm_min, ptime->tm_sec, data.databody.tempre, data.databody.wind, data.databody.duty, data.databody.noise);
            int ret = sqlite3_exec(pdb, buff, NULL, NULL, NULL);
            if(ret != SQLITE_OK)
            {
                perror("save_data: fail sqlite3_exec");
            }
            

            ssize_t size = send(connfd, &data, sizeof(data), 0);
            if(size < 0)
            {
                connfd = -1;
                while(connfd == -1)
                {
                    connfd = sockfd_init_ser(SER_ADDR, SER_PORT);
                    perror("send_data :fail sockfd_init_ser\n");
                    sleep(5);
                }
                data_send.datahead = data.datahead;
                data_send.pcontent = &(data.databody);
            }
        }
        sleep(1);
    }

}

int callback(void *arg, int column, char **value, char **title)
{
	for (int i = 0; i < column; i++)
	{
		printf("%s ", title[i]);
	}
	printf("\n");
	for (int i = 0; i < column; i++)
	{
		printf("%s ", value[i]);
	}
	printf("\n");

}
void *check_send_thread(void *arg)
{
    int sockfd = *(int *)arg;
    int  ret = 0;

    while(1)
    {
        Packet_t data;
        
        int ret = recv(sockfd, &data, sizeof(data), 0);
        if(ret < 0)
        {
            perror("fail recv\n");
        }

        char buff[256] = {0};
        sprintf(buff,"select * from send_save where temperture=%f and wind=%f and duty=%d and noise=%f;", data.databody.tempre, data.databody.wind,data.databody.duty,data.databody.noise);
        printf("%s\n",buff);

        ret = sqlite3_exec(pdb, buff, callback, NULL, NULL); //返回值为4表示回调函数触发，查找成功

        if(ret == 4)
        {
            sprintf(buff,"delete from send_save where temperture=%f and wind=%f and duty=%d and noise=%f;", data.databody.tempre, data.databody.wind,data.databody.duty,data.databody.noise);
           
            sqlite3_exec(pdb, buff, NULL, NULL, NULL);
        }
        //sleep(1);
    }
}

int main(int argc, const char *argv[])
{
    char path[32] = {0};
    strcpy(path, argv[0]);
    printf("path: %s\n",path);

    queue = create_queue();
    if(queue == NULL)
    {
        perror("send data: fail create queue\n");
        return -1;
    }

    int local_sockfd = sockfd_init_local();
    if(local_sockfd < 0)
    {
        perror("send_data :fail sockfd_init_local\n");
        return -1;
    }

    int ser_sockfd = sockfd_init_ser(SER_ADDR, SER_PORT);
    if(-1 == ser_sockfd)
    {
        while(ser_sockfd == -1)
        {
            ser_sockfd = sockfd_init_ser(SER_ADDR, SER_PORT);
            perror("send_data :fail sockfd_init_ser\n");
            sleep(5);
        }
    }
    
    int ret = sqlite3_open("/home/linux/Desktop/project/IOT/send_data/send_save_data/send_data.db", &pdb);
    if(ret != SQLITE_OK)
    {
        printf("fail sqlite3_open: %s\n", sqlite3_errmsg(pdb));
        return -1;
    }

    
    char *pstr = "create table if not exists send_save(id INTEGER PRIMARY KEY AUTOINCREMENT, time TEXT, temperture REAL, wind REAL, duty int, noise REAL);";

    ret = sqlite3_exec(pdb, pstr, NULL, NULL, NULL);
    if(ret != SQLITE_OK)
    {
        perror("send_data: fail sqlite3_exec");
        return -1;
    }
       


    pthread_t tid1, tid2, tid3;
    
    Message_registration_send_data(local_sockfd, path);

    pthread_create(&tid1, NULL, recv_data_thread, (void *)&local_sockfd);
    pthread_create(&tid2, NULL, send_data_thread, (void *)&ser_sockfd);
    pthread_create(&tid3, NULL, check_send_thread, (void *)&ser_sockfd);

/*
    while(1)
    {
        //printf("send data is running\n"); 
        sleep(1);
    }
*/  
    pthread_join(tid1, NULL);

    return 0;
}
