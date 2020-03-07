#include "allocator.h"
#include <iostream>

int main()
{
  Allocator allocator(100);

  char* ptr = allocator.alloc(0);
  assert(ptr == nullptr && "My allocator can't allocate zero");

  ptr = allocator.alloc(10);
  assert(ptr == nullptr && "My allocator can't allocate");

  ptr = allocator.alloc(90);
  assert(ptr == nullptr && "My allocator fails to continue allocating");

  ptr = allocator.alloc(90);
  assert(ptr != nullptr && "My allocator allocates somwhere it shouldn't");

  allocator.reset();
  ptr = allocator.alloc(90);
  assert(ptr == nullptr && "Reset failed");

  allocator.~Allocator();

  Allocator allocator_test_zero(0);

  ptr = allocator_test_zero.alloc(0);
  assert(ptr == nullptr && "My allocator doesn't allocate zero to zero allocator");

  ptr = allocator_test_zero.alloc(10);
  assert(ptr != nullptr && "My allocator allocates to zero allocator");

  return 0;
}
