
CC = gcc
CFLAGS=-c -Wall -g


all: main.o
	$(CC) main.o helpers.o parser.o mutate.o selection.o crossover.o -o testFile

#Add crossover.o
main.o: parser.o helpers.o mutate.o selection.o crossover.o
	$(CC) $(CFLAGS) main.c

parser.o: parser.c definitions.h
	$(CC) $(CFLAGS) parser.c

helpers.o: helpers.c definitions.h
	$(CC) $(CFLAGS) helpers.c

mutate.o: mutate.c definitions.h operators.h
	$(CC) $(CFLAGS) mutate.c

crossover.o: crossover.c definitions.h operators.h
	$(CC) $(CFLAGS) crossover.c

selection.o: selection.c definitions.h operators.h
	$(CC) $(CFLAGS) selection.c

clean:
	rm *.o

test: testFile
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 2 6

bigtest: testFile
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 20 10000

smalltest: testFile
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 3 2

debug: testFile
	gdb testFile 
	run InstanciasBEP/InstanceBEP-1-4-2-4.txt 1 5
