CC=g++
all: test
test: main.o parser.o
	$(CC) -o test main.o parser.o --std=c++14 -O2
main.o: main.cpp parser.h
	$(CC) -c main.cpp --std=c++14 -O2
parser.o: parser.cpp parser.h
	$(CC) -c parser.cpp --std=c++14 -O2
clean:
	rm -rf *.o test
