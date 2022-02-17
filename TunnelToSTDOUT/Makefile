CC = gcc
CFLAGS = -Wall -I 

all: clean tunnel gen_files

tunnel: 
	$(CC) -o tunnel.exe tunnel.c -pthread

gen_files: 
	$(CC) -o gen_files.exe gen_files.c 

clean:
	-rm *.exe *.txt

.PHONY: clean