#include <iostream>

class Allocator {
  size_t maxSize;
  char* begin_ptr;
  char* current_ptr;
public:
  Allocator(size_t maxSize);
  char* alloc(size_t size);
  void reset();
  ~Allocator();
};
