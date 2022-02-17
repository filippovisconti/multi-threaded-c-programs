CC = gcc
CFLAGS = -Wall -I 

all: clean invert gen_data check

invert: 
	$(CC) -o invert.exe invert.c -pthread

gen_data: 
	$(CC) -o gen_data.exe gen_data.c 

check: 
	$(CC) -o check.exe check.c 

clean:
	-rm *.exe *.bin

.PHONY: clean
