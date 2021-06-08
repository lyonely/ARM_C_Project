CC = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
BUILD: assemble

assemble: assemble.o
	$(CC) assemble.o -o ../../out/assemble

clean:
	rm -f *.o assemble
