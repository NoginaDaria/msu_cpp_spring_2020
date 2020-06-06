CC=clang++

all: extsort check

clean:
	rm -f external_sort.exe

extsort:
	$(CC) external_sort.cpp -o external_sort.exe --std=c++14 -O2

check:
	python checker.py
