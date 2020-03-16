#include "parser.h"

OnToken number_callback = nullptr;
OnToken string_callback = nullptr;
OnPosition start_callback = nullptr;
OnPosition end_callback = nullptr;

void register_number_callback(OnToken callback){
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

void parse(const char* text)
{
  if(start_callback != nullptr) start_callback();

  char* text_copy = new char[strlen(text) + 1];
  strcpy(text_copy, text);
  const char* pch = strtok (text_copy, " \n\t");

  while (pch != NULL)
  {
    if ((isdigit(pch[0]))&(number_callback != nullptr)) number_callback(pch);
    else if(string_callback != nullptr) string_callback(pch);

    pch = strtok (NULL, " \n\t");
  }
  if(end_callback != nullptr) end_callback();
  free(text_copy);
}
