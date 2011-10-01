#include "thread.h"

static void printSlots(serverConfig *s)
{
        int sfull=countSlots(s,1), sfree=countSlots(s,0), tot=sfull+sfree;
        printf("* active slots %d/%d\n", sfull,tot);
}

static void printClient(clientSlot *c, bool connected)
{
    printf("* client ");

    if (!connected)
        printf("dis");

    printf("connected %s:%d \n", inet_ntoa(c->caddr.sin_addr),
            ntohs(c->caddr.sin_port));
}

void initThread(serverConfig *s)
{
    threadArg *a = my_malloc(sizeof(threadArg));
    clientSlot *p = my_malloc(sizeof(clientSlot));
    memset(p, 0, sizeof(clientSlot));
    sem_init(&p->sem,0,0);
    sem_wait(&s->lsem);
    p->selected=false;

    insert_back(s->clientlist,(void *)p);
    sem_post(&s->lsem);

    a->srv=s;
    a->cli=p;

    pthread_create(&p->tid, NULL, clientThread,(void*)a);
    printf("* new client thread created \n");
    printSlots(s);
}

void initPool(serverConfig *s)
{
    int i;

    if ( sem_init(&s->lsem, 0, 1) < 0 )
        perrorf("sem_init()");

    for (i=0; i<POOL_SIZE; i++)
        initThread(s);

    printf("* pool of %d threads ready... (max %d)\n\n", POOL_SIZE, MAX_POOL_SIZE);
}

void initSession(clientSlot *c)
{
    strcpy(c->arcname,"Archive");
    strcpy(c->arctype,"gnuzip");

    strcpy(c->host,inet_ntoa(c->caddr.sin_addr));
    c->port=ntohs(c->caddr.sin_port);

    sprintf(c->dir, "%u", (unsigned int)pthread_self());
    if ((c->fd_dir=mkdir(c->dir, 0755)) < 0 )
        perrorf("mkdir()");

    if (opendir(c->dir) == NULL)
        perrorf("opendir()");
}

void closeSession(clientSlot *c)
{
    deltree(c->dir);
    close(c->sock);
    c->sock=0;
}

void sessionLoop(clientSlot *c)
{
    int i, type = 0;
    char *param = (char*) my_malloc(sizeof(char) * (MSGSIZE+1));

    for(i=0;i<MSGSIZE;i++)
        param[i]=0;

    while ( (type = recvCommand(param,&(c->sock))) > 0 )
    {
        if ( (param && strlen(param) == 0) || (param[0] == '\n') )
            executeCommand(type, NULL, c);
        else
            executeCommand(type, param, c);

        for(i=0;i<MSGSIZE;i++)
            param[i]=0;

        fflush(stdout);
    }

    free(param);
}



void *clientThread(void *arg)
{
    threadArg *a = (threadArg*)arg;
    serverConfig *s = a->srv;
    clientSlot *c   = a->cli;

    free(arg);

    while(1)
    {
        sem_wait(&c->sem);
        c->selected=false;

        printClient(c,1);
        printSlots(s);

        initSession(c);
        sessionLoop(c);
        closeSession(c);

        printClient(c,0);
        printSlots(s);

        if ( freeSlot(s, c, MAX_FREE) == 1 )
        {
            printf("* free client thread terminated \n");
            printSlots(s);
            pthread_exit(NULL);

        }
    }

    pthread_exit(NULL);
}
