/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月29日 星期四 19时27分33秒
 ************************************************************************/

#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <pthread.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include "../CenterServer/network.h"
#include"../CenterServer/queue.h"

Queue_t *queue = NULL;
pthread_mutex_t mutex;
char cur_table[32] = {0};
int today = 0;

int print_recv_data(DataType data_recv)
{
    BodyData *data = (BodyData *)data_recv.pcontent;

    printf("save_data: tempre: %.2f",data->tempre);
    printf("  wind: %.2f",data->wind);
    printf("  duty: %.2d",data->duty);
    printf("  noise: %.2f\n\n", data->noise);

    return 0;
}

int socket_init_cli(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("save data: fail socket");
        return -1;
    }

    struct sockaddr_in ser;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(LOCAL_PORT);
    ser.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int ret = connect(sockfd, (struct sockaddr *)&ser, sizeof(ser));
    if(ret < 0)
    {
        perror("fail connect");
        return -1;
    }

    return sockfd;
}

int Message_registration_get_data(int sockfd ,const char *path)
{
    MSG_pack data;
    DataType send_msg;

    send_msg.datahead.agreement_id = 0x03;          //消息注册id 0x03
    send_msg.datahead.size = sizeof(MSG_pack);
    
    data.id = 2;
    data.sockfd = sockfd;
    data.pid = getpid();
    //printf("send_data : %d\n",getpid());
    strcpy(data.path, path);

    send_msg.pcontent = &data;

    send_safety(sockfd, &send_msg);
}

void *recv_data_thread(void *arg)
{
    int connfd = *(int *)arg;
    DataType data_recv;
    Packet_t data;
   
    while(1)
    {
        recv_safety(connfd, &data_recv);
       
        data = *(Packet_t *)&data_recv;
        print_recv_data(data_recv);


        data.datahead = data_recv.datahead;
        data.databody = *(BodyData *)(data_recv.pcontent);

        pthread_mutex_lock(&mutex);
        push_queue(queue, data);
        pthread_mutex_unlock(&mutex); 

        free(data_recv.pcontent);
    }
}

int create_table(sqlite3 *pdb)
{
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
        
        if(today != 0)
        {
            char buff[64] = {0};

            sprintf(buff, "drop table %s;", temp);
            ret = sqlite3_exec(pdb, buff, NULL, NULL, NULL);
            if(ret != SQLITE_OK)
            {
                perror("save_data: fail sqlite3_exec");
                return -1;
            }
        }
        today = ptime->tm_mday;
    }
    return 0;
}

int insert_data(Packet_t data, sqlite3 *pdb)
{
    time_t tim = time(NULL);
    struct tm *ptime = localtime(&tim);

    char buff[256] = {0};

    sprintf(buff,"insert into %s values(NULL, \"%d-%d-%d %d:%d:%d\", %f, %f, %d, %f);",cur_table, ptime->tm_year+1900, ptime->tm_mon+1, ptime->tm_mday, ptime->tm_hour, ptime->tm_min, ptime->tm_sec, data.databody.tempre, data.databody.wind, data.databody.duty, data.databody.noise);

    int ret = sqlite3_exec(pdb, buff, NULL, NULL, NULL);
    if(ret != SQLITE_OK)
    {
        perror("save_data: fail sqlite3_exec");
        return -1;
    }
    
    return 0;
}

void *save_data_thread(void *arg)
{

    Packet_t data;

    sqlite3 *pdb = NULL;
    int ret = sqlite3_open("/home/linux/Desktop/project/IOT/save_data/sqlite/savedata.db", &pdb);
    if(ret != SQLITE_OK)
    {
        printf("fail sqlite3_open: %s\n", sqlite3_errmsg(pdb));
        return NULL;
    }
   
    while(1)
    {
        if(queue->pfront != NULL)
        {
            pthread_mutex_lock(&mutex);
            pop_queue(queue, &data);
            pthread_mutex_unlock(&mutex);
           
            create_table(pdb);
            insert_data(data, pdb);

        }
        sleep(1);
    }
}


int main(int argc, char *argv[])
{
    char path[32] = {0};
    //printf("%s\n",(char *)argv[0]);
    strcpy(path, argv[0]);
    
    int sockfd = socket_init_cli();
    if(sockfd < 0)
    {    
        perror("save_data :fail socket_init_cli\n");
        return -1;
    }

    Message_registration_get_data(sockfd, path);

    queue = create_queue();
    if(NULL == queue)
    {
        perror("save_data :fail create queue");
        return -1;
    }

    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, recv_data_thread, (void *)&sockfd);
    pthread_create(&tid2, NULL, save_data_thread, NULL);

    pthread_join(tid1, NULL);
       
/*
    while(1)
    {
        printf("save data is running\n");
        sleep(1);
    }
*/

    return 0;
}


