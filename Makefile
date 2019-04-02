CC=cc

set: set.o
	$(CC) -o set set.o

get: get.o
	$(CC) -o get get.o

clean:
	rm set.o get.o set get
