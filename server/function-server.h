#ifndef FUNCTION_SERVER_H
#define FUNCTION_SERVER_H

#include "server.h"
#include "archive.h"
#include "../shared/command.h"
#include "../shared/function.h"

bool configureCompressor(char *,void *);
bool configureName(char *,void *);
bool showConfiguration(char *, void *);
bool download(char *,void *);
bool compress(char *,void *);
bool pong(char *, void *);
bool removefile(char *,void *);
bool listfiles(char *, void *);
bool dummy(char *, void *);

#endif
