CC = gcc
CFLAGS = -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99
BUILD: emulate

emulate: emulate.o data_processor.o branch.o shifter.o functions.o multiply.o data_transfer.o
	$(CC) -o emulate emulate.o data_processor.o branch.o shifter.o functions.o multiply.o data_transfer.o 

emulate.o: emulate.c functions.h datatypes.h data_processor.h multiply.h data_transfer.h branch.h
	$(CC) $(CFLAGS) -c emulate.c

data_processor.o: data_processor.c data_processor.h shifter.h functions.h datatypes.h
	$(CC) $(CFLAGS) -c data_processor.c

branch.o: branch.c branch.h functions.h datatypes.h
	$(CC) $(CFLAGS) -c branch.c

functions.o: functions.c functions.h datatypes.h
	$(CC) $(CFLAGS) -c functions.c

shifter.o: shifter.c shifter.h functions.h datatypes.h
	$(CC) $(CFLAGS) -c shifter.c

multiply.o : multiply.c multiply.h functions.h datatypes.h
	$(CC) $(CFLAGS) -c multiply.c

data_transfer.o : data_transfer.c data_transfer.h functions.h shifter.h datatypes.h
	$(CC) $(CFLAGS) -c data_transfer.c

clean:
	rm -f *.o emulate debug
	rm -f emulate
