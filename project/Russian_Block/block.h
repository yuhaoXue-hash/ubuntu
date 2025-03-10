extern unsigned char gBox[4][8];
extern unsigned char gChangeBox[4][8];
extern unsigned char gNextBox[4][8];

extern int InitBox(unsigned char (*pBox)[8]);
extern int CreateBox(unsigned char (*pBox)[8], int BoxNo);
extern int CanMoveBox(unsigned char (*pBox)[8], int y, int x);
extern int MoveBox(unsigned char (*pBox)[8], int y, int x);
extern int ClearBox(unsigned char (*pBox)[8], int y, int x);
extern int GetChangeBoxNo(int No);
