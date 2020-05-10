#include <iostream>
#include <cassert>
#include "pool.h"

int main() {
  ThreadPool pool(8);

  for (int i = 0; i < 10; ++i)
      assert(pool.exec([i]() { return i; }).get() == i);
  std::cout << "OK\n";
}
