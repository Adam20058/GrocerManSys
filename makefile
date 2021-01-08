CC=gcc

CFLAGS=-c -Wall

all: stock.o main.o
	$(CC) stock.o main.o -o main -lm

stock.o: stock.c
	$(CC) $(CFLAGS) stock.c
	
main.o: main.c
	$(CC) $(CFLAGS) main.c
	
clean:
	rm -rf *o main


