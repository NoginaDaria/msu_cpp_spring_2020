#include "allocaror.h"

Allocator::Allocator(std::size_t maxSize)
{
  begin_ptr = new char [maxSize];
  current_ptr = begin_ptr;
}

char* Allocator::alloc(std::size_t size)
{
  if ((current_ptr + size) <= (begin_ptr + maxSize))
  {
    char* res_ptr = current_ptr;
    current_ptr += size;
    return res_ptr;
  } else {
    return nullptr;
  }
}

void Allocator::reset()
{
  current_ptr = begin_ptr;
}

Allocator::~Allocator()
{
  delete [] begin_ptr;
}
