#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include "boarder.h"
#include "block.h"

int cury = 1;
int curx = 16;
int curboxno = 0;
int nextboxno = 0;

void handler(int signo)
{
	ClearBox(gBox, cury, curx);
	if (CanMoveBox(gBox, cury+1, curx))
	{
		MoveBox(gBox, cury+1, curx);
		cury = cury+1;
		ShowMenu();
	}
	else 
	{
		MoveBox(gBox, cury, curx);
		ShowMenu();

		curboxno = nextboxno;
		CreateBox(gBox, nextboxno);
		
		ClearBox(gNextBox, BOARDER_HEIGHT/4-2, BOARDER_WIDTH/6*5-4);
		nextboxno = rand() % 19;
		CreateBox(gNextBox, nextboxno);
		MoveBox(gNextBox, BOARDER_HEIGHT/4-2, BOARDER_WIDTH/6*5-4);

		cury = 1;
		curx = 16;
		if (CanMoveBox(gBox, cury, curx))
		{
			MoveBox(gBox, cury, curx);
			ShowMenu();
		}
		else 
		{
			exit(0);
		}
	}
	
	alarm(1);

	return;
}

int main(void)
{
	char ch = 0;
	signal(SIGALRM, handler);

	srand(time(NULL));
	
	InitBoarder();
	SetBoarder();
	
	curboxno = rand() % 19;
	CreateBox(gBox, curboxno);
	MoveBox(gBox, cury, curx);
	
	nextboxno = rand() % 19;
	CreateBox(gNextBox, nextboxno);
	MoveBox(gNextBox, BOARDER_HEIGHT/4-2, BOARDER_WIDTH/6*5-4);

	ShowMenu();
	alarm(1);

	while (1)
	{
		ch = getchar();
		if ('a' == ch || 'A' == ch)
		{
			ClearBox(gBox, cury, curx);
			if (CanMoveBox(gBox, cury, curx-2))
			{
				MoveBox(gBox, cury, curx-2);
				ShowMenu();
				curx -= 2;
			}
			else 
			{
				MoveBox(gBox, cury, curx);
				ShowMenu();
			}
		}
		else if ('w' == ch || 'W' == ch)
		{
			ClearBox(gBox, cury, curx);
			CreateBox(gChangeBox, GetChangeBoxNo(curboxno));
			if (CanMoveBox(gChangeBox, cury, curx))
			{
				MoveBox(gChangeBox, cury, curx);
				memcpy(gBox, gChangeBox, sizeof(gChangeBox));
				curboxno = GetChangeBoxNo(curboxno);
				ShowMenu();
			}
			else 
			{
				MoveBox(gBox, cury, curx);
				ShowMenu();
			}
		}
		else if ('d' == ch || 'D' == ch)
		{
			ClearBox(gBox, cury, curx);
			if (CanMoveBox(gBox, cury, curx+2))
			{
				MoveBox(gBox, cury, curx+2);
				ShowMenu();
				curx += 2;
			}
			else 
			{
				MoveBox(gBox, cury, curx);
				ShowMenu();
			}
		}
		else if ('s' == ch || 'S' == ch)
		{
			ClearBox(gBox, cury, curx);
			if (CanMoveBox(gBox, cury+1, curx))
			{
				MoveBox(gBox, cury+1, curx);
				ShowMenu();
				cury += 1;
			}
			else 
			{
				MoveBox(gBox, cury, curx);
				ShowMenu();
			}
		}
		else if ('p' == ch || 'P' == ch)
		{
			alarm(0);
		}
		else if ('c' == ch || 'C' == ch)
		{
			alarm(1);
		}
	}

	return 0;
}
