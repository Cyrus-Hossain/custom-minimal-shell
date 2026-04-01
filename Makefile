CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -O2 -g

all: myshell

myshell: main.o parser.o executor.o builtins.o job.o
	$(CC) $(CFLAGS) -o myshell main.o parser.o executor.o builtins.o job.o

main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c shell.h parser.h
	$(CC) $(CFLAGS) -c parser.c

executor.o: executor.c shell.h executor.h
	$(CC) $(CFLAGS) -c executor.c

builtins.o: builtins.c shell.h builtins.h
	$(CC) $(CFLAGS) -c builtins.c

job.o: job.c shell.h job.h
	$(CC) $(CFLAGS) -c job.c

clean:
	rm -f *.o myshell
