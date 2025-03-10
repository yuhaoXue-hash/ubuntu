/***********************************************************
 * 
 * 文件名:boarder.c 
 * 功能: 游戏界面相关功能
 * 
 *
 ***********************************************************/

#include <stdio.h>
#include "boarder.h"

/* 游戏界面的数据内容 */
unsigned char gBoarder[BOARDER_HEIGHT][BOARDER_WIDTH];

/* 初始化游戏界面数组内容 */
int InitBoarder(void)
{
	int i = 0;
	int j = 0;
	
	//1.对所有的元素都赋值为' '
	for (j = 0; j < BOARDER_HEIGHT; j++)
	{
		for (i = 0; i < BOARDER_WIDTH; i++)
		{
			gBoarder[j][i] = ' ';
		}
	}

	return 0;
}

/* 游戏界面加入边框 */
int SetBoarder(void)
{
	int i = 0;
	int j = 0;

	//1.第一行赋值为'#'
	for (i = 0; i < BOARDER_WIDTH; i++)
	{
		gBoarder[0][i] = '#';
	}

	//2.最后一行赋值为'#'
	for (i = 0; i < BOARDER_WIDTH; i++)
	{
		gBoarder[BOARDER_HEIGHT-1][i] = '#';
	}

	//3.第一列赋值为'#'
	for (j = 0; j < BOARDER_HEIGHT; j++)
	{
		gBoarder[j][0] = '#';
		gBoarder[j][1] = '#';
	}

	//4.最后一列赋值为'#'
	for (j = 0; j < BOARDER_HEIGHT; j++)
	{
		gBoarder[j][BOARDER_WIDTH-1] = '#';
		gBoarder[j][BOARDER_WIDTH-2] = '#';
	}

	//5.界面中间选择一列赋值为'#',作为游戏区域和其他功能区域的划分
	for (j = 0; j < BOARDER_HEIGHT; j++)
	{
		gBoarder[j][BOARDER_WIDTH/3*2-2] = '#';
		gBoarder[j][BOARDER_WIDTH/3*2-1] = '#';
	}
	
	//6.游戏界面有方一行的一部分赋值为'#'
	for (i = BOARDER_WIDTH/3*2; i < BOARDER_WIDTH; i++)
	{
		gBoarder[BOARDER_HEIGHT/2][i] = '#';
	}

	return 0;
}

/* 游戏界面的显示 */
int ShowMenu(void)
{
	int i = 0;
	int j = 0;
	
	printf("\033[2J");
	for (j = 0; j < BOARDER_HEIGHT; j++)
	{
		for (i = 0; i < BOARDER_WIDTH; i++)
		{
			if ('#' == gBoarder[j][i])
			{
				printf("\033[41;31m%c\033[0m", gBoarder[j][i]);
			}
			else if ('[' == gBoarder[j][i] || ']' == gBoarder[j][i])
			{
				printf("\033[40;30m%c\033[0m", gBoarder[j][i]);
			}
			else
			{
				printf("%c", gBoarder[j][i]);
			}
		}
		printf("\n");
	}

	return 0;
}
