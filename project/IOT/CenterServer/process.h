/*************************************************************************
	> File Name: process.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年08月20日 星期二 11时20分34秒
 ************************************************************************/

#ifndef _PROCESS_H
#define _PROCESS_H

#include "cJSON.h"
#include "unistd.h"

#define  JSON_FILE_SIZE   1024

extern cJSON *json_pare(const char *paths);
extern int execl_child(const char *paths);
extern int process_init(const char *path);

#endif
