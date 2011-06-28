#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdbool.h>
#include <errno.h>

#include "file.h"
#include "time.h"
#include "utility.h"

#define BLOCK 8192

bool senddir(const char *, unsigned int *);
bool pushfile(const char *, unsigned int *);
bool pullfile(const char *, unsigned int *);

#endif
