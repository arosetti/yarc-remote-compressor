#ifndef COMMAND_C_H
#define COMMAND_C_H

#include "function-client.h"

void commandLoop(clientConfig *);
bool executeCommand (char *name, char *param, clientConfig *);

#endif
