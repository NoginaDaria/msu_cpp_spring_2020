#include<allocator.h>

int main()
{
  // create allocator
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

  return 0;
}
