#ifndef FUNCTION_CLIENT_H
#define FUNCTION_CLIENT_H

#include "client.h"
#include "../shared/function.h"

bool help(char *, void *);
bool configureCompressor(char *, void *);
bool configureName(char *, void *);
bool showConfiguration(char *, void *);
bool upload(char *, void *);
bool compress(char *, void *);
bool quit(char *, void *);
bool ping(char *, void *);
bool removefile(char *, void *);
bool listfiles(char *, void *);
bool connectSrv(char *, void *);
bool dummy(char *, void *);

#endif
