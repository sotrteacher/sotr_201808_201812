CC=gcc
CFLAGS=-lpthread

all:mutex


thrdcreat:mutex.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -vf mutex
