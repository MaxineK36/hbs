
CC = gcc
CFLAGS = -Wall -g -Wextra
SRC = src/main.c src/features.c
OBJ = shell

all:
	$(CC) $(CFLAGS) -o $(OBJ)  
clean:
	rm $(OBJ)
