#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

#ifndef THREAD_H
#define THREAD_H

#include "command-server.h"
#include "list.h"

void initThread(serverConfig *);
void initPool(serverConfig *);
void *clientThread(void *);
void *slotsThread(void *);
void closeSession(clientSlot *);

#endif
