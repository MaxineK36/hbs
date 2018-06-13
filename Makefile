CC = gcc
CFLAGS = -fPIC -Wall -g -Wextra -std=c99 -D_GNU_SOURCE -I./lib/log.c/src
SRC = src/main.c src/features.c lib/log.c/src/log.c
OBJ = shell

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ) -lreadline
clean:
	rm $(OBJ)
