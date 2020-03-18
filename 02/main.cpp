#include "parser.h"

void onNumberFound(int n){
  std::cout << n << '\n';
}
void onStringFound(std::string n){
  std::cout << "I found a string " << n << '\n';
}
void Start(){
  std::cout << "yay, let's start\n";
}
void End(){
  std::cout << "It's high time to know that it works perfectly\n";
}

int main()
{
  register_number_callback(onNumberFound);
  register_string_callback(onStringFound);
  register_start_callback(Start);
  register_end_callback(End);

  const std::string input_string = "123 abc 4567";
  parse(input_string);

  return 0;
}
