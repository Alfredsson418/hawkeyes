#pragma once

#include <cjson/cJSON.h>
#include <stdio.h>

#include "print_macro.h"

void print_file(char *file, int width);

void print_json(char *file, int width, char *key);
