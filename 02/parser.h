#include <string.h>
#include <iostream>

#pragma once

using OnToken = void (*)(std::string token);
using OnTokenNum = void (*)(int token);
using OnPosition = void (*)();

void parse(const std::string &text);
void register_number_callback(OnTokenNum callback);
void register_string_callback(OnToken callback);
void register_start_callback(OnPosition callback);
void register_end_callback(OnPosition callback);
