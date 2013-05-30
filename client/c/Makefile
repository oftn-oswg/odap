CC = gcc
CFLAGS = -O3 -Wall -Werror -Wextra -std=c99 -pedantic

.PHONY: all clean

all: libodap.so

odap.o: odap.h
	$(CC) -fpic $(CFLAGS) -c odap.c

libodap.so: odap.o
	$(CC) -shared -o libodap.so odap.o

clean:
	-@rm -rf odap.o libodap.so
	
