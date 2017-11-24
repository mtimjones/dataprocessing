CC = gcc
CFLAGS = -I. -g -std=c99 -Wall

all: cleanse

cleanse: cleanse.c parse.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f cleanse
