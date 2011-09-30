#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#include "../../config.h"

void *my_malloc(int);
void my_perror(char*, bool);

void cleanTailSpaces(char *);
void stepProgressBar(long ,int ,long);
void num2human(long double);

#endif
