
OBJ=save_data
SCR+=main.o
SCR+=../CenterServer/queue.o
SCR+=../CenterServer/network.o
SCR+=../CenterServer/control.o
SCR+=../CenterServer/message.o
CC=gcc
INCLUDE=./
FLAG=-lm -lpthread -lsqlite3 

$(OBJ):$(SCR)
	$(CC) $^ -o $@ -I$(INCLUDE) $(FLAG) -g

main.o:main.c
	$(CC) -c $^ -o $@ -I$(INCLUDE) $(FLAG)
add.o:add.c
	$(CC) -c $^ -o $@ -I$(INCLUDE) $(FLAG)

%.o:%.c
	$(CC) -c $^ -o $@ -I$(INCLUDE) $(FLAG)

clean:
	rm $(OBJ) $(SCR)


