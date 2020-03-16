#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#pragma once

using OnToken = void (*)(const char* token);
using OnPosition = void (*)();

void parse(const char* text);
void register_number_callback(OnToken callback);
void register_string_callback(OnToken callback);
void register_start_callback(OnPosition callback);
void register_end_callback(OnPosition callback);

OnToken number_callback = nullptr;
OnToken string_callback = nullptr;
OnPosition start_callback = nullptr;
OnPosition end_callback = nullptr;
