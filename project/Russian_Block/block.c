/***********************************************************
 * 
 * 文件名:block.c 
 * 功能: 俄罗斯方块功能
 * 
 *
 ***********************************************************/

#include <stdio.h>
#include "block.h"
#include "boarder.h"

/* 俄罗斯方块数据 */
unsigned char gBox[4][8];

/* 俄罗斯方块变形后数据 */
unsigned char gChangeBox[4][8];

/* 俄罗斯方块下一个方块数据 */
unsigned char gNextBox[4][8];

/* 初始化俄罗斯方块内容 */
int InitBox(unsigned char (*pBox)[8])
{
	int i = 0;
	int j = 0;

	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 8; i++)
		{
			pBox[j][i] = ' ';
		}
	}

	return 0;
}

/* 俄罗斯方块变形规则 */
int GetChangeBoxNo(int No)
{
	int DstNo = -1;
	
	switch (No)
	{
		case 0:DstNo = 1;break;
		case 1:DstNo = 0;break;
		case 2:DstNo = 2;break;
		case 3:DstNo = 4;break;
		case 4:DstNo = 5;break;
		case 5:DstNo = 6;break;
		case 6:DstNo = 3;break;
		case 7:DstNo = 8;break;
		case 8:DstNo = 9;break;
		case 9:DstNo = 10;break;
		case 10:DstNo = 7;break;
		case 11:DstNo = 12;break;
		case 12:DstNo = 13;break;
		case 13:DstNo = 14;break;
		case 14:DstNo = 11;break;
		case 15:DstNo = 16;break;
		case 16:DstNo = 15;break;
		case 17:DstNo = 18;break;
		case 18:DstNo = 17;break;
	}

	return DstNo;
}

/* 创建俄罗斯方块图形 */
int CreateBox(unsigned char (*pBox)[8], int BoxNo)
{
	int i = 0;
	int j = 0;

	InitBox(pBox);

	if (0 == BoxNo)
	{
		//[][][][]
		//
		//
		//

		for (i = 0; i < 8; i++)
		{
			pBox[0][i] = (0 == i % 2 ? '[' : ']');
		}
	}
	else if (1 == BoxNo)
	{
		//[]
		//[]
		//[]
		//[]
		
		for (j = 0; j < 4; j++)
		{
			pBox[j][0] = '[';
			pBox[j][1] = ']';
		}
	}
	else if (2 == BoxNo)
	{
		//[][]
		//[][]
		//
		//

		for (j = 0; j < 2; j++)
		{
			for (i = 0; i < 4; i++)
			{
				pBox[j][i] = (i % 2 == 0 ? '[' : ']');
			}
		}
	}
	else if (3 == BoxNo)
	{
		//[][]
		//[]
		//[]
		//

		for (j = 0; j < 3; j++)
		{
			pBox[j][0] = '[';
			pBox[j][1] = ']';
		}
		pBox[0][2] = '[';
		pBox[0][3] = ']';
	}
	else if (4 == BoxNo)
	{
		//[][][]  
		//    []
		//
		//

		for (i = 0; i < 6; i++)
		{
			pBox[0][i] = (0 == i % 2 ? '[' : ']');
		}
		pBox[1][4] = '[';
		pBox[1][5] = ']';
	}
	else if (5 == BoxNo)
	{
		//  []
		//  []
		//[][]
		//

		for (j = 0; j < 3; j++)
		{
			pBox[j][2] = '[';
			pBox[j][3] = ']';
		}
		pBox[2][0] = '[';
		pBox[2][1] = ']';
	}
	else if (6 == BoxNo)
	{
		//[]
		//[][][]
		//
		//

		for (i = 0; i < 6; i++)
		{
			pBox[1][i] = (0 == i % 2 ? '[' : ']');
		}
		pBox[0][0] = '[';
		pBox[0][1] = ']';
	}
	else if (7 == BoxNo)
	{
		//[][]
		//  []
		//  []
		//

		for (j = 0; j < 3; j++)
		{
			pBox[j][2] = '[';
			pBox[j][3] = ']';
		}
		pBox[0][0] = '[';
		pBox[0][1] = ']';
	}
	else if (8 == BoxNo)
	{
		//    []
		//[][][] 
		//
		//

		for (i = 0; i < 6; i++)
		{
			pBox[1][i] = (0 == i % 2 ? '[' : ']');
		}
		pBox[0][4] = '[';
		pBox[0][5] = ']';
	}
	else if (9 == BoxNo)
	{
		//[]
		//[]
		//[][]
		//

		for (j = 0; j < 3; j++)
		{
			pBox[j][0] = '[';
			pBox[j][1] = ']';
		}
		pBox[2][2] = '[';
		pBox[2][3] = ']';
	}
	else if (10 == BoxNo)
	{
		//[][][]
		//[]
		//
		//

		for (i = 0; i < 6; i++)
		{
			pBox[0][i] = (0 == i % 2 ? '[' : ']');
		}
		pBox[1][0] = '[';
		pBox[1][1] = ']';
	}
	else if (11 == BoxNo)
	{
		//  []
		//[][][]
		//
		//

		for (i = 0; i < 6; i++)
		{
			pBox[1][i] = (0 == i % 2 ? '[' : ']');
		}
		pBox[0][2] = '[';
		pBox[0][3] = ']';
	}
	else if (12 == BoxNo)
	{
		//[]
		//[][]
		//[]
		//

		for (j = 0; j < 3; j++)
		{
			pBox[j][0] = '[';
			pBox[j][1] = ']';
		}
		pBox[1][2] = '[';
		pBox[1][3] = ']';
	}
	else if (13 == BoxNo)
	{
		//[][][]
		//  []
		//
		//

		for (i = 0; i < 6; i++)
		{
			pBox[0][i] = (0 == i % 2 ? '[' : ']');
		}
		pBox[1][2] = '[';
		pBox[1][3] = ']';
	}
	else if (14 == BoxNo)
	{
		//  []
		//[][]
		//  []
		//

		for (j = 0; j < 3; j++)
		{
			pBox[j][2] = '[';
			pBox[j][3] = ']';
		}
		pBox[1][0] = '[';
		pBox[1][1] = ']';
	}
	else if (15 == BoxNo)
	{
		//[]
		//[][]
		//  []
		//

		for (j = 0; j < 2; j++)
		{
			pBox[j][0] = '[';
			pBox[j][1] = ']';
		}
		for (j = 1; j < 3; j++)
		{
			pBox[j][2] = '[';
			pBox[j][3] = ']';
		}
	}
	else if (16 == BoxNo)
	{
		//  [][]
		//[][]
		//
		//

		for (i = 2; i < 6; i++)
		{
			pBox[0][i] = (0 == i % 2 ? '[' : ']');
		}
		for (i = 0; i < 4; i++)
		{
			pBox[1][i] = (0 == i % 2 ? '[' : ']');
		}
	}
	else if (17 == BoxNo)
	{
		//  []
		//[][]
		//[]
		//

		for (j = 1; j < 3; j++)
		{
			pBox[j][0] = '[';
			pBox[j][1] = ']';
		}
		for (j = 0; j < 2; j++)
		{
			pBox[j][2] = '[';
			pBox[j][3] = ']';
		}
	}
	else if (18 == BoxNo)
	{
		//[][]
		//  [][]
		//
		//

		for (i = 0; i < 4; i++)
		{
			pBox[0][i] = (0 == i % 2 ? '[' : ']');
		}
		for (i = 2; i < 6; i++)
		{
			pBox[1][i] = (0 == i % 2 ? '[' : ']');
		}
	}

	return 0;
}

/* 判断俄罗斯方块是否能够移动到指定游戏界面位置 */
int CanMoveBox(unsigned char (*pBox)[8], int y, int x)
{
	int i = 0;
	int j = 0;

	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 8; i++)
		{
			if ((pBox[j][i] == '[' || pBox[j][i] == ']') && (gBoarder[j+y][i+x] != ' '))
			{
				return 0;
			}
		}
	}

	return 1;
}

/* 将俄罗斯方块移动到游戏界面中 */
int MoveBox(unsigned char (*pBox)[8], int y, int x)
{
	int i = 0;
	int j = 0;

	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 8; i++)
		{
			if (pBox[j][i] == '[' || pBox[j][i] == ']')
			{
				gBoarder[j+y][i+x] = pBox[j][i];
			}
		}
	}

	return 0;
}

/* 将俄罗斯方块从游戏界面中清除 */
int ClearBox(unsigned char (*pBox)[8], int y, int x)
{
	int i = 0;
	int j = 0;

	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 8; i++)
		{
			if (pBox[j][i] == '[' || pBox[j][i] == ']')
			{
				gBoarder[j+y][i+x] = ' ';
			}
		}
	}

	return 0;
}
