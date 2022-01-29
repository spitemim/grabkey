CC=gcc
CFLAGS=-Wall -std=c89 -pedantic -g
LDFLAGS=-lX11

all: grabkey

grabkey: grabkey.c
	$(CC) $(CFLAGS) grabkey.c $(LDFLAGS) -o grabkey

clean:
	rm grabkey
