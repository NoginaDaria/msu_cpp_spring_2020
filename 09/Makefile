CC=clang++

all: extsort check

clean:
	rm -f external_sort.exe
	rm new_sorted_file

extsort:
	$(CC) external_sort.cpp -o external_sort.exe --std=c++14 -O2

check:
	python checker.py
