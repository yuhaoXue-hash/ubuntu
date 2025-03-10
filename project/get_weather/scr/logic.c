/*************************************************************************
	> File Name: logic.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月30日 星期二 11时36分55秒
 ************************************************************************/

#include<stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include "logic.h"
#include <unistd.h>

int day = 0;
char filename[32] = {0};

int logic_init(void)
{
    DIR *pdir = opendir("logic");
    if(NULL == pdir)
    {
        int mkd = mkdir("./logic", 0777);
        if(mkd == -1)
        {
            perror("fail mkdir\n");
            return -1;
        }
        pdir = opendir("logic");
        if(NULL == pdir)
        {
            perror("fail opendir\n");
            return -1;
        }
    }

    return 0;
}

int logic_create(void)
{
    time_t tim = time(NULL);
    struct tm *localtim = localtime(&tim);
    
    if(day != localtim->tm_mday)
    {
        day = localtim->tm_mday;
        sprintf(filename,"logic/%d-%d-%d_logic.txt",localtim->tm_year+1900, localtim->tm_mon+1, localtim->tm_mday);
        
        int fd = open(filename, O_RDWR|O_CREAT|O_APPEND,0664);
        if(-1 == fd)
        {
            perror("fail open\n");
            return -1;
        }
        close(fd);
    }
    
    return 0;
}

int logic_write(int num ,char *str)
{
    char time_data[64] = {0};

    int fd = open(filename, O_RDWR|O_APPEND, 0664);
    if(fd == -1)
    {
        perror("fail open\n");
        return -1;
    }
    
    time_t tim = time(NULL);
    struct tm *localtim = localtime(&tim);

    if(num == 1)
    {
        char modul_str[32] = {0};
        int *pnum = (int *)str;
        if(*pnum == 1)
        {
            strcpy(modul_str, "1.实时天气");
        }
        else if(*pnum == 2)
        {
            strcpy(modul_str, "2.未来天气"); 
        }
 
        sprintf(time_data, "[%d-%d-%d %d:%d:%2d]用户天气模式选择:%s\n",localtim->tm_year+1900, localtim->tm_mon+1, localtim->tm_mday, localtim->tm_hour,localtim->tm_min, localtim->tm_sec,modul_str);
    }
    else if(num == 2)
    {
        sprintf(time_data, "[%d-%d-%d %d:%d:%2d]用户城市选择:%s\n",localtim->tm_year+1900, localtim->tm_mon+1, localtim->tm_mday, localtim->tm_hour,localtim->tm_min, localtim->tm_sec,str); 
    }
    else if(num == 3)
    {
        sprintf(time_data, "[%d-%d-%d %d:%d:%d]用户天气模式选择:%s\n",localtim->tm_year+1900, localtim->tm_mon+1, localtim->tm_mday, localtim->tm_hour,localtim->tm_min, localtim->tm_sec,str); 
    }
    write(fd, time_data, strlen(time_data));
    
    close(fd);

    return 0;
}


