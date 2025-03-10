/*************************************************************************
	> File Name: log.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: Mon 27 May 2024 06:11:08 PM CST
 ************************************************************************/

#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <time.h>

#define LOG_INFO    1
#define LOG_WARING  2
#define LOG_ERROR   3

extern int log_Init(void);
extern int Log_Dir_Test(void);
extern int log_write(int level, char *INFO);
extern int log_Deinit(void);

typedef struct log
{
    FILE *fp;
    struct tm tim;
}LOG_T;

#endif
