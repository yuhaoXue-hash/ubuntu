/*************************************************************************
	> File Name: log.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年09月05日 星期四 21时56分48秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include "log.h"

Log_t log;

int log_init(void)
{
    DIR *pd = opendir("Log");
    if(NULL == pd)
    {
        mkdir("log", 0777);    
    }

    int ret = chdir("Log");
    if(ret == 0)
    {
        printf("change direction error!\n");
        return -1;
    }

    return 0;
}

int log_create_dir(void)
{
    char direction_name[256] = {0};

    if(log.month != log.ptime->tm_mon)
    {
        log.month = log.ptime->tm_mon;
        sprintf(direction_name,"%d-%02d-%02d",log.ptime->tm_year+1900, log.ptime->tm_mon+1, log.ptime->tm_mday);
        mkdir(direction_name, 0777);
        int ret = chdir(direction_name);
        if(ret == 0)
        {
            printf("change direction error!\n");
            return -1;
        }
    }

    return 0;
}

int log_create_file(void)
{
    char filename[256] = {0};

    if(log.day != log.ptime->tm_mday)
    {
        log_Deinit(log.fp);
        log.day = log.ptime->tm_mday;
        sprintf(filename, "log%d-%d-%d %02d:%02d.txt",log.ptime->tm_year+1900, log.ptime->tm_mon+1, log.ptime->tm_mday, log.ptime->tm_hour, log.ptime->tm_min);
        log.fp = fopen(filename,"a");
        if(NULL == log.fp)
        {
            printf("fileopen fail!\n");
            return -1;
        }
    
    }
}

int log_write(int level, char *INFO)
{
    time_t sec;
    struct tm *pt = NULL;

    time(&sec);
    pt = localtime(&sec);

    fprintf(log.fp, "[%d-%d-%d %02d:%02d:%02d][%d] %s\n",pt->tm_year+1900, pt->tm_mon+1, pt->tm_mday,pt->tm_hour,pt->tm_min,pt->tm_sec, level, INFO);    
    
    fflush(log.fp);
}

int log_Deinit(void)
{
    if(log.fp != NULL)
    {
        fclose(log.fp);
    }
}

