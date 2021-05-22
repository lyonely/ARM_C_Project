CC = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
BUILD: emulate

emulate: emulate.o

data_processor: data_processor.o datatypes.h functions.h

clean:
	rm -f *.o emulate
