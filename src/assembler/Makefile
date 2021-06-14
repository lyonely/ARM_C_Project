CC = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
BUILD: main

main: main.o
	$(CC) main.o -o ../../out/assemble

main.o: main.c assemble.h datatypes.h
	$(CC) $(CFLAGS) -c main.c

assemble.o: assemble.c assemble.h datatypes.h parser.h symboltable.h
	$(CC) $(CFLAGS) -c assemble.c

parser.o: parser.c parser.h datatypes.h functions.h symboltable.h
	$(CC) $(CFLAGS) -c parser.c

symboltable.o: symboltable.c symboltable.h datatypes.h
	$(CC) $(CFLAGS) -c symboltable.c

functions.o: functions.c functions.h datatypes.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o ../../out/assemble
	
