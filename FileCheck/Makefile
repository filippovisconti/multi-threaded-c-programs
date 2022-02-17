CC = gcc
CFLAGS = -Wall -g -I 

all: fc gfc

fc: 
	$(CC) -o fc fc.c -pthread

gfc: 
	$(CC) -o gfc gfc.c -pthread

clean:
	-rm fc gfc lock

.PHONY: clean
