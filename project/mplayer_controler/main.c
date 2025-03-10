/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月24日 星期三 11时18分05秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>
#include "play.h"
#include"screen.h"

int fd = 0;
int flag_sreen = 0;
int song_num = 0;
int song_num_max = 0;
int model_flag = 1;
int end_flag = 0;    //  1是他杀，0是自然消亡;

extern char song_name[10][128];



PLAY_STAT_t flag_g = PLAY_FREE;



int get_user_input(void)
{
    int num = 0;
    scanf("%d",&num);

    return num;
}

void show_menu()
{
    if(flag_sreen == 0)
    {
        first_sreen();
    }
    else if(flag_sreen == 1)
    {
        player_list();
    }
    else if(flag_sreen == 2)
    {
        speed_choose();
    }
    else if(flag_sreen == 3)
    {
        location();
    }
    else if(flag_sreen == 4)
    {
        player_model();
    }
}

void do_ctl(int num)
{
    char buff[128] = {0};

    if(flag_sreen == 0)
    {
        switch(num)
        {
            case 1: flag_sreen = 1;break;
            case 2: start_play(song_num);pause_function(); break;
            case 3: stop_function();break;
            case 4: last_song(); break;
            case 5: next_song(); break;
            case 6: speed_choose_ctl(); break;
            case 7: seek_fun();break;
            case 8: model_choose();break;
            case 9: quit_functuon();break;
        }
    }
    else if(flag_sreen == 1)
    {
        if(flag_g == PLAY_FREE)
        {
            start_play(num);
        }
        else
        {
            stop_function();
            start_play(num);
        }
        flag_sreen = 0;
        flag_g = PLAY_ON;
        song_num = num;
    }
    else if(flag_sreen == 2)
    {
        switch(num)
        {
            case 1: write(fd, "speed_set 1\n",12); break;
            case 2: write(fd, "speed_set 2\n", strlen("speed_set 2\n")); break;
            case 3: write(fd, "speed_set 4\n", strlen("speed_set 4\n")); break;
        }
        flag_sreen = 0;
    }
    else if(flag_sreen == 3)
    {
        sprintf(buff ,"seek %d 1\n",num);
        write(fd, buff, strlen(buff));
        flag_sreen = 0;
    }
    else if(flag_sreen == 4)
    {
        model_flag = num;
        flag_sreen = 0;
    }
}



int main()
{
    int num = 0;
    
    int ret = mkfifo("./fifo", 0664);
    if(ret < 0 && errno != EEXIST)
    {
        perror("fail mkfifo\n");
        return -1;
    }

    song_num_max = get_song();

    while(1)
    {
        clear_sreen();
        show_menu();
        num = get_user_input();
        do_ctl(num);
    }


    return 0;

}
