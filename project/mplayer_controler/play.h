/*************************************************************************
	> File Name: play.h
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月24日 星期三 14时45分27秒
 ************************************************************************/

#ifndef _PLAY_H
#define _PLAY_H

typedef enum
{
    PLAY_ON,
    PLAY_PAUSE,
    PLAY_FREE,
}PLAY_STAT_t;

extern PLAY_STAT_t flag_g;

extern int fd;
extern int flag_sreen;
extern int song_num;
extern int song_num_max;
extern int model_flag;
extern int end_flag;   

extern char song_name[10][128];

extern int get_song(void);
extern int start_play(int i);
extern int last_song(void);
extern int next_song(void);
extern int speed_choose_ctl(void);
extern int seek_fun(void);
extern void model_choose(void);
extern void pause_function(void);
extern void stop_function(void);
extern void quit_function(void);

#endif
