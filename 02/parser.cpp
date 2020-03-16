#include "parser.h"

void (*number_callback)(const char* token);
void (*string_callback)(const char* token);
void (*start_callback)();
void (*end_callback)();

bool number_callback_bool = false;
bool string_callback_bool = false;
bool start_callback_bool = false;
bool end_callback_bool = false;

void register_number_callback(OnToken callback){
  OnToken number_callback = callback;
  number_callback_bool = true;
}
void register_string_callback(OnToken callback){
  OnToken string_callback = callback;
  string_callback_bool = true;
}
void register_start_callback(OnPosition callback){
  OnPosition start_callback = callback;
  start_callback_bool = true;
}
void register_end_callback(OnPosition callback){
  OnPosition end_callback = callback;
  end_callback_bool = true;
}

void parse(const char* text)
{
  if(start_callback_bool) start_callback();

  char* text_copy = new char[strlen(text) + 1];
  strcpy(text_copy, text);
  char* pch = strtok (text_copy, " \n\t");

  while (pch != NULL)
  {
    if ((isdigit(pch[0]))&(number_callback_bool)) number_callback(pch);
    else if(string_callback_bool) string_callback(pch);

    pch = strtok (NULL, " \n\t");
    std::cout << pch << '\n';
  }
  if(end_callback_bool) end_callback();
  free(text_copy);
}
