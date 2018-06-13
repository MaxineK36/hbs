CC = gcc
CFLAGS = -Wall -g -Wextra -std=c99 -D_GNU_SOURCE
SRC = src/main.c src/features.c
OBJ = shell

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ) -lreadline
clean:
	rm $(OBJ)
