#include "allocator.h"
#include <iostream>

int main()
{
  Allocator allocator(100);

  char* ptr = allocator.alloc(0);
  assert(ptr != nullptr && "My allocator can't allocate zero");

  ptr = allocator.alloc(10);
  assert(ptr != nullptr && "My allocator can't allocate");

  ptr = allocator.alloc(90);
  assert(ptr != nullptr && "My allocator fails to continue allocating");

  ptr = allocator.alloc(90);
  assert(ptr == nullptr && "My allocator allocates somewhere it shouldn't");

  allocator.reset();

  ptr = allocator.alloc(90);
  assert(ptr != nullptr && "Reset failed");

  ptr = allocator.alloc(10);
  assert(ptr != nullptr && "My allocator can't allocate after reset");

  return 0;
}
