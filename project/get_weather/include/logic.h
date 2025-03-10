/*************************************************************************
	> File Name: logic.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月30日 星期二 13时01分02秒
 ************************************************************************/

#ifndef _LOGIC_H
#define _LOGIC_H

extern int day;
extern char filename[32];

extern int logic_init(void);
extern int logic_create(void);
extern int logic_write(int num, char *str);

#endif

