/// @file Utils.h

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <math.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MANGETA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define STRANGE "\x1b[1;35;7m"

#define COLOR_PRINT(color, ...)  {printf (color __VA_ARGS__); printf (RESET);}                      ///< Colorful print

#define COLOR_FPRINT(file, color, ...) {fprintf (file, color __VA_ARGS__); fprintf (file, RESET);}

#endif ///not UTILS_H
