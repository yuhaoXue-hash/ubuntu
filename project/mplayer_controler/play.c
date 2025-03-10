
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<signal.h>
#include"play.h"

char song_name[10][128];

void handle(int sig)
{
    if(end_flag == 0)
    {
        flag_g = PLAY_FREE;
        if(model_flag == 1)
        {
            song_num++;
            if(song_num > (song_num_max-1))
            {
                song_num = 0;
            }
            start_play(song_num);
        }
        else if(model_flag == 2)
        {
            start_play(song_num);
        }
        else if(model_flag == 3)
        {
            start_play(rand()%song_num_max);
        }
    }
    end_flag = 0;
}

int get_song(void)
{
    int i = 0;

    DIR *pdir = opendir("./song/");
    if(pdir == NULL)
    {
        printf("opendir fail");
        return -1;
    }
    struct dirent *pdirent = readdir(pdir);

    while(pdirent != NULL)
    {
        if(pdirent->d_name[0] == '.')
        {
            pdirent = readdir(pdir);
            continue;
        }
        strcpy(song_name[i], pdirent->d_name);
        i++;
        pdirent = readdir(pdir);
    }

    return i;
}

int start_play(int i)
{
    char buff[128] = {0};
   
    signal(SIGCHLD, handle);
    if(flag_g == PLAY_FREE)
    {
        flag_g = PLAY_ON;
        pid_t pid = fork();
        if(pid > 0)
        {
            fd = open("./fifo", O_WRONLY);
            if(fd < 0)
            {
                perror("fail open\n");
                return -1;
            }          
        }
        else if(pid == 0)
        {
            close(1);
            close(2);
            sprintf(buff,"/home/linux/Desktop/%s",song_name[i]);
            execl("/usr/bin/mplayer","mplayer","-slave","-input","file=fifo",buff,NULL); 
        }
        else
        {
            printf("fork fail\n");
            return -1;
        }
    }
    return 0;
}

int last_song(void)
{
    if(song_num < (song_num_max-1))
    {
        song_num++;
    }
    else
    {
        song_num = 0;
    }
    
    if(flag_g == PLAY_FREE)
    {
        start_play(song_num);
    }
    else
    {
        stop_function();
        start_play(song_num);
    }
    flag_g = PLAY_ON;
}

int next_song(void)
{
    if(song_num > 0)
    {
        song_num--;
    }
    else
    {
        song_num = song_num_max-1;
    }

    if(flag_g == PLAY_FREE)
    {
        start_play(song_num);
    }
    else
    {
        stop_function();
        start_play(song_num);
    }
    flag_g = PLAY_ON;
}

int speed_choose_ctl(void)
{
    if(flag_g == PLAY_ON)
    {
        flag_sreen = 2;       
    }
}

int seek_fun(void)
{
    if(flag_g == PLAY_ON)
    {
        flag_sreen = 3;       
    } 
}

void model_choose(void)
{
    flag_sreen = 4;
}

void quit_functuon(void)
{
    write(fd, "quit\n",strlen("quit\n"));
    end_flag = 1;
    flag_g = PLAY_FREE;
}

void pause_function(void)
{
    if(flag_g != PLAY_FREE)
    {
        if(flag_g == PLAY_ON)
        {
            flag_g = PLAY_PAUSE;
        }
        else if(flag_g == PLAY_PAUSE)
        {
            flag_g = PLAY_ON;
        }
        write(fd, "pause\n",6);
    }
    else
    {
        flag_g = PLAY_ON;
    }
}

void stop_function(void)
{
    if(flag_g != PLAY_FREE)
    {
        write(fd, "stop\n",strlen("stop\n"));
        flag_g = PLAY_FREE;
        end_flag = 1;
    }
}

