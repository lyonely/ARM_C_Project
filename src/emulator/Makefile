CC = gcc
CFLAGS = -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99
BUILD: emulate debug

emulate: emulate.o

data_processor.o: data_processor.c data_processor.h shifter.h functions.c functions.h datatypes.h

branch.o: branch.c functions.c functions.h datatypes.h

functions.o: functions.c functions.h datatypes.h

debug: debug.o data_processor.o functions.o shifter.o

debug.o: data_processor.c data_processor.h shifter.h functions.h datatypes.h

shifter.o: shifter.c shifter.h functions.h datatypes.h

clean:
	rm -f *.o emulate debug

