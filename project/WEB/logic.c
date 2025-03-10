/*************************************************************************
	> File Name: logic.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月06日 星期二 12时54分24秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int cur_day = 0;
char filename[32] = {0};

int init_logic(void)
{
    DIR *pdir = opendir("./logic");
    if(pdir == NULL)
    {
        int ret = mkdir("./logic", 0777);
        if(ret == -1)
        {
            perror("fail mkdir\n");
            return -1;
        }
        pdir = opendir("./logic");
    }
    
    return 0;
}

int create_logic(void)
{
    time_t tim = time(NULL);

    struct tm *ltime =localtime(&tim);
    if(ltime == NULL)
    {
        perror("get localtime fail\n");
        return -1;
    }
    
    if(cur_day = ltime->tm_mday)
    {
        sprintf(filename ,"./logic/logic:%d_%02d_%02d.txt",ltime->tm_year+1900, ltime->tm_mon+1, ltime->tm_mday);
        int fd = open(filename, O_CREAT|O_RDWR|O_APPEND, 0664);
        if(fd == -1)
        {
            perror("fail open\n");
            return -1;
        }
        close(fd);
    }

    return 0;
}

int write_logic(const char *p, const char *ip)
{
    char buff[256] = {0};
    
    time_t tim = time(NULL);

    struct tm *ltime =localtime(&tim);
    if(ltime == NULL)
    {
        perror("get localtime fail\n");
        return -1;
    }
    
    int fd = open(filename, O_CREAT|O_RDWR|O_APPEND, 0664);
    if(fd == -1)
    {
        perror("fail open\n");
        return -1;
    }
    
    sprintf(buff, "%s[%d-%02d-%02d %02d:%02d:%02d] %s\n",ip, ltime->tm_year+1900, ltime->tm_mon+1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec, p);

    int ret = write(fd, buff, strlen(buff));
    
    return 0;
}



