CC = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
BUILD: main

main: main.o
	$(CC) main.o -o ../../out/assemble

main.o: main.c assemble.h datatypes.h
	$(CC) $(CFLAGS) -c main.c

assemble.o: assemble.c assemble.h datatypes.h
	$(CC) $(CFLAGS) -c assemble.c

datap_assembler.o: datap_assembler.c datap_assembler.h datatypes.h
	$(CC) $(CFLAGS) -c datap_assembler.c

multiply_assembler.o: multiply_assembler.c multiply_assembler.h datatypes.h
	$(CC) $(CFLAGS) -c multiply_assembler.c

branch_assembler.o: branch_assembler.c branch_assembler.h datatypes.h
	$(CC) $(CFLAGS) -c branch_assembler.c

sdt_assembler.o: sdt_assembler.c sdt_assembler.h datatypes.h
	$(CC) $(CFLAGS) -c sdt_assembler.c

functions.o: functions.c functions.h datatypes.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o ../../out/assembler
	
