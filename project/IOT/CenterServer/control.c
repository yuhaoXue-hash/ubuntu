/*************************************************************************
	> File Name: control.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月21日 星期三 15时34分47秒
 ************************************************************************/

#include<stdio.h>
#include "control.h"
#include "message.h"
#include "network.h"

extern int ep_ret;

//////    MsgTable
//   消息表id对应数组下标
//
//      0     采集线程
//      1
//      2
//      3
///////

int print_message_table(MSG_pack MsgTable)
{
    printf("id = %d  ",MsgTable.id);
    printf("sockfd = %d  ",MsgTable.sockfd);
    printf("pid = %d  ",MsgTable.pid);
    printf("path = %s\n",MsgTable.path);

    return 0;
}

int get_message(DataType data, int sockfd)
{
    MSG_pack *pmsg = (MSG_pack *)data.pcontent;


    MsgTable[pmsg->id] = *pmsg;
    MsgTable[pmsg->id].sockfd = sockfd;
    
    for(int i = 0; i < 4; i++)
    {
        print_message_table(MsgTable[i]);
    }

    return 0;
}



int print_get_data(DataType *data_recv)
{
    BodyData *data = (BodyData *)data_recv->pcontent;
    

    for(int i = 1; i < 4; i++)
    {
        int sockfd = MsgTable[i].sockfd;
        if(sockfd != 0)
        {
            send_safety(sockfd, data_recv);
        }
        
    }

}

