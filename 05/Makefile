CC=clang++
all: test
test: main.o bigint.o
	$(CC) -o test main.o bigint.o --std=c++14 -O2
main.o: main.cpp bigint.h
	$(CC) -c main.cpp --std=c++14 -O2
bigint.o: bigint.cpp bigint.h
	$(CC) -c bigint.cpp --std=c++14 -O2
clean:
	rm -rf *.o test
