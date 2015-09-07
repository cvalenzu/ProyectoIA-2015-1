CC = gcc
CFLAGS=-c -Wall -g


all: main.o
	$(CC) main.o helpers.o parser.o mutate.o selection.o crossover.o -o BEP

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
 
test1: BEP
	time ./BEP InstanciasBEP/InstanceBEP-1-4-2-4.txt 100 10000

test2: BEP
	time ./BEP InstanciasBEP/InstanceBEP-1-5-3-6.txt 100 10000

test3: BEP
	time ./BEP InstanciasBEP/InstanceBEP-2-12-3-6.txt 100 10000

test4: BEP
	time ./BEP InstanciasBEP/InstanceBEP-2-22-4-10.txt 100 10000

test5: BEP
	time ./BEP InstanciasBEP/InstanceBEP-2-32-5-18.txt 100 10000

test6: BEP
	time ./BEP InstanciasBEP/InstanceBEP-2-9-7-5.txt 100 10000

test7: BEP
	time ./BEP InstanciasBEP/InstanceBEP-3-11-10-7.txt 100 10000

test8: BEP
	time ./BEP InstanciasBEP/InstanceBEP-5-25-12-15.txt 100 10000

test9: BEP
	time ./BEP InstanciasBEP/InstanceBEP-8-40-20-20.txt 100 10000





