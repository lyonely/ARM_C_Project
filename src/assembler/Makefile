CC = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
BUILD: assemble

assemble: assemble.o
	$(CC) assemble.o -o ../../out/assemble

assemble.o: assemble.c datatypes.h

datap_assembler.o: datap_assembler.c datap_assembler.h datatypes.h

functions.o: functions.c functions.h datatypes.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o assemble
	
