CC=clang++
all: test
test: main.o matrix.o
	$(CC) -o test main.o matrix.o --std=c++14 -O2
main.o: main.cpp matrix.h
	$(CC) -c main.cpp --std=c++14 -O2
matrix.o: matrix.cpp matrix.h
	$(CC) -c matrix.cpp --std=c++14 -O2
clean:
	rm -rf *.o test
