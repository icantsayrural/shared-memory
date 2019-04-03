CC=cc

set: set.o shared.h shmhelper.o
	$(CC) -o set set.o shmhelper.o

get: get.o shared.h shmhelper.o
	$(CC) -o get get.o shmhelper.o

clean:
	rm set.o get.o set get shmhelper.o
