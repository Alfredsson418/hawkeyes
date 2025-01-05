#pragma once

#include <string.h>

#include "display_file.h"
#include "print_macro.h"

#define TERMINAL_WIDTH 70

void ui_line(char *text, char padding, int width);
