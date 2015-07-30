
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
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 20 2

longtest: testFile
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 20 10000

bigtest: testFile
	./testFile InstanciasBEP/InstanceBEP-8-40-20-20.txt 50 20000

smalltest: testFile
	./testFile InstanciasBEP/InstanceBEP-1-5-3-6.txt 2 100

test1: testFile
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 10 100

test2: testFile
	./testFile InstanciasBEP/InstanceBEP-1-5-3-6.txt 10 100

test3: testFile
	./testFile InstanciasBEP/InstanceBEP-2-12-3-6.txt 10 100

test4: testFile
	./testFile InstanciasBEP/InstanceBEP-2-22-4-10.txt 10 100

test5: testFile
	./testFile InstanciasBEP/InstanceBEP-2-32-5-18.txt 10 100

test6: testFile
	./testFile InstanciasBEP/InstanceBEP-2-9-7-5.txt 10 100

test7: testFile
	./testFile InstanciasBEP/InstanceBEP-3-11-10-7.txt 10 100

test8: testFile
	./testFile InstanciasBEP/InstanceBEP-5-25-12-15.txt 10 100

test9: testFile
	./testFile InstanciasBEP/InstanceBEP-8-40-20-20.txt 10 100





