/*************************************************************************
	> File Name: control.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月30日 星期二 11时28分55秒
 ************************************************************************/

#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "cJSON.h"
#include "control.h"
#include"control.h"

void screen_model_choose(void)
{
    printf("1.实时天气\n");
    printf("2.天气预报\n");
    printf("3.退出\n");
    printf("输入数字查看天气：");
}

void screen_name_input(void)
{
    printf("请输入要查看的城市名：");
}

int recv_http_response(int sockfd)
{

    while(1)
    {
        ssize_t size = recv(sockfd, recv_buff, sizeof(recv_buff)-1, 0);
        if(size < 0)
        {
            perror("fail recv\n");
            return -1;
        }
        if(size == 0)
        {
            break;
        }
    }

    int i= 0,j = 0,stat = 0;
    while(recv_buff[i] != '\0')
    {
        if(recv_buff[i] == '{' && stat == 0)
        {
            stat = 1;        
        }
        if(stat == 1)
        {
            json_buff[j] = recv_buff[i];
            j++;
            i++;
        }
        else
        {                   
            i++;
        }
    }
    return 0;
}

int send_http_Realtime(int sockfd,char *cityNM)
{
    char preq[2048] = {0};

    sprintf(preq,"""GET /?app=weather.today&cityNm=%s&appkey=73540&sign=3dbb81621287e78d4e8bbf4efff8b173&format=json HTTP/1.1\r\n"
            "Host: api.k780.com\r\n"
            "Connection: close\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.71 Safari/537.36 Core/1.94.265.400 QQBrowser/12.7.5765.400\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
            "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n""",cityNM);

    ssize_t size = send(sockfd, preq, strlen(preq), 0);
    if(size < 0)
    {
        perror("send fail\n");
        return -1;
    }
    
    return 0;
}

int send_http_future(int sockfd, char *cityNM)
{
    char preq[2048] = {0};
    sprintf(preq,"""GET /?app=weather.future&cityNm=%s&appkey=73540&sign=3dbb81621287e78d4e8bbf4efff8b173&format=json HTTP/1.1\r\n"
            "Host: api.k780.com\r\n"
            "Connection: close\r\n"
            "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.71 Safari/537.36 Core/1.94.265.400 QQBrowser/12.7.5765.400\r\n"
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n"
            "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n""",cityNM);
   
    ssize_t size = send(sockfd, preq, strlen(preq), 0);
    if(size < 0)
    {
        perror("send fail\n");
        return -1;
    }
  
    return 0;
}


int json_display(int input_num)
{
    int i = 0;

    cJSON* cjson = cJSON_Parse(json_buff);
    if(cjson == NULL)
    {
        printf("jason pack into json error...\n");
        return -1;
    }
    
    if(!cJSON_IsObject(cjson))
    {
        printf("没有对象\n");
        cJSON_Delete(cjson);
        return -1;
    }

    cJSON *result = cJSON_GetObjectItem(cjson, "result");
    if(result == NULL)
    {
        printf("get result fail\n");
        cJSON_Delete(cjson);
        return -1; 
    }

    if(input_num == 1)
    {
        char *days = cJSON_GetObjectItem(result, "days")->valuestring;
        char *week = cJSON_GetObjectItem(result, "week")->valuestring;
        char *citynm = cJSON_GetObjectItem(result, "citynm")->valuestring;
        char *temperature = cJSON_GetObjectItem(result, "temperature")->valuestring;
        char *temperature_curr = cJSON_GetObjectItem(result, "temperature_curr")->valuestring;
        char *humidity = cJSON_GetObjectItem(result, "humidity")->valuestring;
        char *weather = cJSON_GetObjectItem(result, "weather")->valuestring;
        char *weather_curr = cJSON_GetObjectItem(result, "weather_curr")->valuestring;
        char *wind = cJSON_GetObjectItem(result, "wind")->valuestring;
        char *winp = cJSON_GetObjectItem(result, "winp")->valuestring;
    
    
        printf("日期：%s\n",days);
        printf("星期：%s\n",week);
        printf("城市名：%s\n",citynm);
        printf("温度区间：%s\n",temperature);
        printf("当前温度：%s\n",temperature_curr);
        printf("湿度：%s\n",humidity);
        printf("今日天气：%s\n",weather);
        printf("当前天气：%s\n",weather_curr);
        printf("风向：%s\n",wind);
        printf("风力：%s\n",winp);
        printf("\n");
    }
    else
    {
        int arraysize = cJSON_GetArraySize(result);
        cJSON *result_pArr = cJSON_GetArrayItem(result, 0);
        if(NULL == result_pArr)
        {
            perror("fail get array\n");
            cJSON_Delete(cjson);
            return -1;
        }

        while(1)
        {       
            char *days = cJSON_GetObjectItem(result_pArr, "days")->valuestring;
            char *week = cJSON_GetObjectItem(result_pArr, "week")->valuestring;
            char *citynm = cJSON_GetObjectItem(result_pArr, "citynm")->valuestring;
            char *temperature = cJSON_GetObjectItem(result_pArr, "temperature")->valuestring;
            char *weather = cJSON_GetObjectItem(result_pArr, "weather")->valuestring;
            char *wind = cJSON_GetObjectItem(result_pArr, "wind")->valuestring;
            char *winp = cJSON_GetObjectItem(result_pArr, "winp")->valuestring;

            printf("日期：%s\n",days);
            printf("星期：%s\n",week);
            printf("城市名：%s\n",citynm);
            printf("温度区间：%s\n",temperature);
            printf("今日天气：%s\n",weather);
            printf("风向：%s\n",wind);
            printf("风力：%s\n",winp);
            printf("\n");

            i++;
            if(i >= arraysize)
            {
                break;
            }
            result_pArr = cJSON_GetArrayItem(result, i);
        }
    }

    cJSON_Delete(cjson);
    return 0;
}


