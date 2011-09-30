#include "socket-client.h"

int clientSetup(clientConfig *c)
{
    errno=0;
    if ((c->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perrorf("socket()");
        return c->sock;
    }

    memset(&c->srvaddr, 0, sizeof(c->srvaddr));
    c->srvaddr.sin_addr.s_addr = inet_addr(c->host);
    c->srvaddr.sin_family = AF_INET;
    c->srvaddr.sin_port = htons(c->port);
    c->connected=false;

    return 0;
}

int clientConnect(clientConfig *c)
{
    int ret;

    if ((ret = connect(c->sock, (struct sockaddr*)&c->srvaddr,
                       sizeof(struct sockaddr))) < 0)
    {
        c->connected=false;
        printf("* connection failed, %s:%d\n", c->host, c->port);
        perrorf("connect()");
    }
    else
    {
        c->connected=true;
        printf("* connected with %s:%d\n", c->host, c->port);
    }

    return ret;
}
