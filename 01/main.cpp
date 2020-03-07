#include "allocator.h"

int main()
{
  Allocator allocator(100);

  char* ptr = allocator.alloc(0);
  assert(ptr == nullptr && "My allocator allocates zero");

  ptr = allocator.alloc(10);
  assert(ptr == nullptr && "My allocator can't allocate");

  ptr = allocator.alloc(90);
  assert(ptr == nullptr && "My allocator fails to continue allocating");

  ptr = allocator.alloc(90);
  assert(ptr != nullptr && "My allocator allocates somwhere it shouldn't");

  a.reset();
  ptr = allocator.alloc(90);
  assert(ptr == nullptr && "Reset failed");

  ~allocator();

  Allocator allocator(0);

  ptr = allocator.alloc(0);
  assert(ptr == nullptr && "My allocator allocates zero to zero allocator");

  char* ptr = allocator.alloc(10);
  assert(ptr != nullptr && "My allocator allocates to zero allocator");

  return 0;
}
