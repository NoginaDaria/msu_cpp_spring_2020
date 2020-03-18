#include "parser.h"

OnTokenNum number_callback = nullptr;
OnToken string_callback = nullptr;
OnPosition start_callback = nullptr;
OnPosition end_callback = nullptr;

std::string delimiter = " \n\t";
std::string pch;
bool nonstop = true;
size_t last;
size_t next;

void register_number_callback(OnTokenNum callback){
  number_callback = callback;
}
void register_string_callback(OnToken callback){
  string_callback = callback;
}
void register_start_callback(OnPosition callback){
  start_callback = callback;
}
void register_end_callback(OnPosition callback){
  end_callback = callback;
}

void parse(const std::string &text)
{
  if(start_callback != nullptr) start_callback();

  last = 0;
  next = 0;
  do{
    if((next = text.find_first_of(delimiter, last)) != std::string::npos){
      pch = text.substr(last, next-last);
    }else{
      pch = text.substr(last);
      nonstop = false;
    }
    if ((isdigit(pch[0]))&&(number_callback != nullptr)) number_callback(stoi(pch));
    else if(string_callback != nullptr) string_callback(pch);
    last = next + 1;
  } while (nonstop);
  if(end_callback != nullptr) end_callback();
}
