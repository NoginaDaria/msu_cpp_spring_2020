#include "allocator.h"
#include <iostream>

int main()
{
  Allocator allocator(100);

  char* ptr = allocator.alloc(0);
  assert(ptr != nullptr && "My allocator can't allocate zero");
  // begin_ptr = 0, current_ptr = 0
  ptr = allocator.alloc(10);
  assert(ptr != nullptr && "My allocator can't allocate");
  // begin_ptr = 0, current_ptr = 10
  ptr = allocator.alloc(90);
  assert(ptr != nullptr && "My allocator fails to continue allocating");
  // begin_ptr = 0, current_ptr = 100
  // 100+90 <= 0+100
  ptr = allocator.alloc(90);
  assert(ptr == nullptr && "My allocator allocates somwhere it shouldn't");

  allocator.reset();

  ptr = allocator.alloc(90);
  assert(ptr != nullptr && "Reset failed");

  ptr = allocator.alloc(10);
  assert(ptr == nullptr && "My allocator can't allocate after reset");

  allocator.~Allocator();

  return 0;
}
