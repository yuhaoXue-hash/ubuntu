/*************************************************************************
	> File Name: string_process.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月15日 星期一 17时29分19秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include"string_process.h"

char string[64] = {0};
char *pcut[5] = {NULL};

int get_string(void)
{
    int len = 0;

    fgets(string, 64, stdin);
    len = strlen(string);
    string[len-1] = '\0';

    return 0;
}

int string_cut(void)
{
    int i = 0; 
    char *pstr = NULL;
    
    pstr = strtok(string," ");
    while(pstr != NULL)
    {
        pcut[i] = pstr;  
        i++;
        pstr = strtok(NULL, " ");
    }

    /*

    for(i = 0; i < 5; i++)
    {
        printf("pcut[%d] =%s\n",i,pcut[i]);
    }
    */

    return i;
}

int find_string(int cnt)
{
    int flag = 0;

    if(strcmp(pcut[0],"touch") == 0)
    {
        flag = 1;    
    }
    else if(strcmp(pcut[0],"cp") == 0)
    {
        flag = 2;
    }
    else if(strcmp(pcut[0],"mv") == 0)
    {
        flag = 3;
    }
    else if(strcmp(pcut[0],"ls") == 0 && cnt  != 1)
    {
        if(strcmp(pcut[1],"-a") == 0)
        {
            flag = 4;
        }
        else if(strcmp(pcut[1], "-l") == 0)
        {
            flag = 5;
        }
    }
    else if(strcmp(pcut[0],"mkdir") == 0)
    {
        flag = 6;
    }
    else if(strcmp(pcut[0],"rmdir") == 0)
    {
        flag = 7;
    }
    else if(strcmp(pcut[0],"pwd") == 0)
    {
        flag = 8;
    }
    else if(strcmp(pcut[0],"cd") == 0)
    {
        flag = 9;
    }
    else if(strcmp(pcut[0],"ln") == 0)
    {
        if(strcmp(pcut[1],"-s") == 0)
        {
            flag = 11;
        }
        else if(cnt == 3)  
        {
            flag = 10;
        }
    }
    else if(cnt == 1)
    {
        if(strcmp(string ,"exit") == 0)
        {
            flag = 12;
        }
        else if(strcmp(string,"ls") == 0)
        {
            flag = 13;
        }
        else if(strcmp(string,"pwd") == 0)
        {
            flag = 15;
        }
    }
    return flag;
}

int string_process(void)
{
    int flag = 0;
    int cnt = 0;

    get_string();
    cnt = string_cut();
    flag = find_string(cnt);

    return flag;
}

int clean_strdata(void)
{
    int i = 0;
    int len = sizeof(string)/sizeof(string[0]);

    for(i = 0; i < 5; i++)
    {
        pcut[i] = NULL;
    }
    for(i = 0; i < len; i++)
    {
        string[i] = '\0';
    }
}







