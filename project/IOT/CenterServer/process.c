/*************************************************************************
	> File Name: process.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月20日 星期二 09时59分42秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "process.h"
#include "public.h"


cJSON *json_pare(const char *paths)
{
    char JsonBuff[JSON_FILE_SIZE] = {0};

    int fd = open(paths, O_RDONLY);
    if(fd < 0)
    {
        perror("fail to open");
        return NULL;
    }

    int curFileSize = -1, allFileSize = 0;
    while(curFileSize != 0)
    {
        curFileSize = read(fd, JsonBuff, (JSON_FILE_SIZE-allFileSize));
        allFileSize += curFileSize;
    }
    printf("%s",JsonBuff);

    cJSON *root = cJSON_Parse(JsonBuff);

    return root;
}

int execl_child(const char *paths)
{
    execl(paths,(void *)paths ,NULL);
    printf("execl error");
}

int process_init(const char *path)
{
    cJSON *root = json_pare(path);

    cJSON *pmodule = cJSON_GetObjectItem(root, "MODULE");
    int ArraySize = cJSON_GetArraySize(pmodule);
    printf("ArraySize = %d\n",ArraySize);
        
    pid_t pid[ArraySize];

    for(int i = 0; i < ArraySize; i++)
    {
        pid[i] = fork();
        if(pid[i] < 0)
        {
            perror("process init: fail fork");
            return -1;
        }
        else if(pid[i]  == 0)
        {
            cJSON *paths = cJSON_GetObjectItem(cJSON_GetArrayItem(pmodule,i), "PATH");
            cJSON *delay = cJSON_GetObjectItem(cJSON_GetArrayItem(pmodule,i), "DELAY");
            //printf("paths = %s\n", paths->valuestring);
            //printf("delay = %d\n", delay->valueint);
            if(cJSON_IsString(paths) && (paths->valuestring != NULL))
            {
                if(cJSON_IsNumber(delay))
                {
                    usleep(delay->valueint);
                }
                execl_child(paths->valuestring);
            }
            exit(0);
        }
        else
        {
        
        }
    }

}
