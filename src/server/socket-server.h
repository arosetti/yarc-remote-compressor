#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <pthread.h>
#include <semaphore.h>

#include "../shared/utility.h"
#include "../shared/command.h"
#include "../shared/list.h"

#define BACKLOG          10
#define POOL_SIZE         2
#define MAX_FREE          1
#define MAX_POOL_SIZE   128

typedef struct serverConfig
{
    int port;
    unsigned int sock;
    struct sockaddr_in my_addr;
    sem_t lsem;
    list *clientlist;
} serverConfig;

typedef struct clientSlot
{
    unsigned int sock;
    struct sockaddr_in caddr;
    char host[16];
    int port;
    bool connected,selected;

    pthread_t tid;
    sem_t sem;

    char dir[256];
    int fd_dir;
    char arcname[256];
    char arctype[16];
} clientSlot;

typedef struct threadArg
{
    serverConfig *srv;
    clientSlot   *cli;
} threadArg;

void serverLoop(serverConfig *);
void serverSetup(serverConfig *);
int  countSlots(serverConfig *, bool);
int  freeSlot(serverConfig *, clientSlot *, int);

#endif




