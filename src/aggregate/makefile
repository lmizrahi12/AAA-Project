CC=g++
CFLAGS=-fopenmp

makebacktracking: backtracking.o
	$(CC) -o backtracking backtracking.o $(CFLAGS)

makeaggregateCSV: aggregateCSV.o
	$(CC) -o aggregateCSV aggregateCSV.o $(CFLAGS)

clean:
	rm *.o
