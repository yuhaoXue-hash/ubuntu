/*************************************************************************
	> File Name: screen.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月25日 星期四 09时20分23秒
 ************************************************************************/

#ifndef _SCREEN_H
#define _SCREEN_H

extern char song_name[10][128];
extern int model_flag;

extern void clear_sreen(void);
extern void first_sreen(void);
extern void player_list(void);       //1
extern void speed_choose(void);    //2
extern void location(void);    //3
extern void player_model(void);   //4

#endif
