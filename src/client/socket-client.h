#ifndef SOCKET_C_H
#define SOCKET_C_H

#include "../shared/command.h"

typedef struct clientConfig {
    char host[32];
    int port;
    unsigned int sock;
    struct sockaddr_in srvaddr;
    bool connected;
} clientConfig;

int clientSetup(clientConfig *);
int clientConnect(clientConfig *);

#endif
