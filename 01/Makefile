CC=g++
all: run_allocator
run_allocator: main.o allocator.o
	$(CC) -o run_allocator main.o allocator.o --std=c++14 -O2
main.o: main.cpp allocator.h
	$(CC) -c main.cpp --std=c++14 -O2
allocator.o: allocator.cpp allocator.h
	$(CC) -c allocator.cpp --std=c++14 -O2
clean:
	rm -rf *.o run_allocator
