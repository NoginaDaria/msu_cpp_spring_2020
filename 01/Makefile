CC=g++
all: run_allocator
run_allocator: main.o allocator.o
	$(CC) -o run_allocator main.o allocator.o
main.o: main.cpp allocator.h
	$(CC) -c main.cpp
allocator.o: allocator.cpp allocator.h
	$(CC) -c allocator.cpp
clean:
	rm -rf *.o run_allocator
test:
	valgrind ./run_allocator
