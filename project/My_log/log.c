/*************************************************************************
	> File Name: log.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: Mon 27 May 2024 06:10:25 PM CST
 ************************************************************************/
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>       
#include <dirent.h>
#include <time.h>
#include <unistd.h>


#include "log.h"

LOG_T log_g;

int Log_Dir_Test(void)
{
    DIR *pd = opendir("Log");
    if(NULL == pd)
    {
        mkdir("Log",0777);
    }

    int ret = chdir("Log");
    if(ret < 0)
    {
        printf("change dirction error!\n");
        return -1;
    }
    
    return 0;
}

int log_Init(void)
{
    time_t sec = 0;
    struct tm *ptim = NULL;
    char filename[256] = {0};

    time(&sec);
    ptim = localtime(&sec);
    log_g.tim = *ptim;
    
    sprintf(filename, "log%d-%d-%d %02d:%02d.txt",ptim->tm_year+1900, ptim->tm_mon+1, ptim->tm_mday, ptim->tm_hour, ptim->tm_min);
    
    log_g.fp = fopen(filename,"a");
    if(NULL == log_g.fp)
    {
        printf("fileopen fail!\n");
        return -1;
    }

    return 0;
}

int log_write(int level, char *INFO)
{
    time_t sec;
    struct tm *pt = NULL;

    time(&sec);
    pt = localtime(&sec);

    if(log_g.tim.tm_min != pt->tm_min)
    {
        log_Deinit();
        log_Init();
    }

    fprintf(log_g.fp, "[%d-%d-%d %02d:%02d:%02d][%d] %s\n",pt->tm_year+1900, pt->tm_mon+1, pt->tm_mday,pt->tm_hour,pt->tm_min,pt->tm_sec, level, INFO);    
    
    fflush(log_g.fp);
}

int log_Deinit(void)
{
    if(log_g.fp != NULL)
    {
        fclose(log_g.fp);
    }
}


