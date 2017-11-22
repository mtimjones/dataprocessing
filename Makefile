CC = gcc
CFLAGS = -I. -std=c99 -Wall

all: cleanse

cleanse: cleanse.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f cleanse
