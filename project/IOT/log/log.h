/*************************************************************************
	> File Name: log.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年09月05日 星期四 22时32分32秒
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#include <time.h>
#include <stdio.h>

typedef struct log
{
    FILE *fp; 
    int month;
    int day;
    struct tm *ptime;
}Log_t;


#endif
