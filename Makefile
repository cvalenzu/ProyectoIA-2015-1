all: main.o
	gcc main.o helpers.o parser.o mutate.o crossover.o selection.o -o testFile

main.o: parser.o helpers.o mutate.o crossover.o selection.o
	gcc -c main.c

parser.o: parser.c definitions.h
	gcc -c parser.c

helpers.o: helpers.c definitions.h
	gcc -c helpers.c

mutate.o: mutate.c definitions.h operators.h
	gcc -c mutate.c

crossover.o: crossover.c definitions.h operators.h
	gcc -c crossover.c

selection.o: selection.c definitions.h operators.h
	gcc -c selection.c

clean:
	rm *.o

test: testFile
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 10 10

bigtest: testFile
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 10 1000

smalltest: testFile
	./testFile InstanciasBEP/InstanceBEP-1-4-2-4.txt 3 5
