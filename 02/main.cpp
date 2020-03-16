#include "parser.h"
#include <iostream>

void onNumberFound(const char * n){
  std::cout << n << '\n';
}

int main()
{
  register_number_callback(onNumberFound);
  parse("123 abc 4567");

  return 0;
}
