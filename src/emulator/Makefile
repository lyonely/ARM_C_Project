CC = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99
BUILD: emulate data_processor

emulate: emulate.o

data_processor: data_processor.o functions.o

data_processor.o: data_processor.c data_processor.h functions.h datatypes.h

functions.o: functions.c datatypes.h

clean:
	rm -f *.o emulate data_processor

