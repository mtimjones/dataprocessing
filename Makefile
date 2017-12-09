CC = gcc
CFLAGS = -I. -g -std=c99 -Wall

all: cleanse learn

cleanse: cleanse.c parse.c summary.c
	$(CC) -o $@ $^ $(CFLAGS)

learn: learn.c vq.c art.c
	$(CC) -o $@ $^ $(CFLAGS) -lm

clean:
	rm -f cleanse learn
