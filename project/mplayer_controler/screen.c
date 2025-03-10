/*************************************************************************
	> File Name: screen.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月25日 星期四 09时18分30秒
 ************************************************************************/

#include<stdio.h>
#include"screen.h"

void clear_sreen(void)
{
    int i = 0;
    for(i = 0; i < 15; i++)
    {
        printf("\n");
    }
}

void first_sreen(void)
{
    printf("+------------------+\n");
    printf("|    视频播放器    |\n");
    printf("+------------------+\n");
    printf("|  1.查看播放列表  |\n");
    printf("|  2.开始、暂停    |\n");
    printf("|  3.停止          |\n");
    printf("|  4.上一个        |\n");
    printf("|  5.下一个        |\n");
    printf("|  6.倍速播放      |\n");
    printf("|  7.定位播放      |\n");
    printf("|  8.播放方式      |\n");
    printf("|  9.退出          |\n");
    printf("+------------------+\n");
    printf("选择功能：");
}

void player_list(void)       //1
{    
    int i = 0;

    printf("+--------------------+\n");
    printf("|     视频播放器     |\n");
    printf("+--------------------+\n");
    while(song_name[i][0] != '\0')
    {
        printf("|%s|\n",song_name[i]);
        i++;
    }
    printf("+--------------------+\n");
    printf("选择视频：");
}

void speed_choose(void)    //2
{
    printf("+--------------------+\n");
    printf("|     视频播放器     |\n");
    printf("+--------------------+\n");
    printf("|      1.1倍速       |\n");
    printf("|      2.2倍速       |\n");
    printf("|      3.4倍速       |\n");
    printf("+--------------------+\n");
    printf("选择倍速：\n");
}

void location(void)    //3
{
    printf(" +-------------------+\n");
    printf("|     视频播放器     |\n");
    printf("+--------------------+\n");
    printf("定位：");
}


void player_model(void)   //4
{
    printf("+--------------------+\n");
    printf("|     视频播放器     |\n");
    printf("+--------------------+\n");
    printf("|1.顺序播放          |\n");
    printf("|2.单曲循环          |\n");
    printf("|3.随机播放          |\n");
    printf("+--------------------+\n");
    printf("|当前模式为：");
    if(model_flag == 1)
    {
        printf("顺序播放\n");
    }
    else if(model_flag == 2)
    {
        printf("单曲循环\n");
    }
    else
    {
        printf("随机播放\n");
    }
    printf("选择模式：");
}
