#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>

#include "../../config.h"

#define BSIZE 1024

void *my_malloc(int);
void perrorf(const char*, ...);

void cleanTailSpaces(char *);
void stepProgressBar(long ,int ,long);
void num2human(long double);

#endif
