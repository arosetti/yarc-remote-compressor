#include "server.h"
#include "thread.h"

int countSlots(serverConfig *s, bool count_full)
{
    list *l;
    elem* p;
    clientSlot *c=NULL;
    int n=0;

    sem_wait(&s->lsem);

    l=s->clientlist;
    p = l->head;

    while (p)
    {
        c=(clientSlot *)p->p;

        if (count_full  && (c->sock != 0) )
            n++;

        if (!count_full && (c->sock == 0))
            n++;

        p = p->next;
    }

    sem_post(&s->lsem);

    return n;
}

int freeSlot(serverConfig *s, clientSlot *ckill, int maxfree)
{
    list *l;
    elem* p, *ekill=NULL;
    clientSlot *c=NULL;
    int n=0;

    sem_wait(&s->lsem);

    l=s->clientlist;
    p = l->head;

    while (p)
    {
        if (p->p == ckill)
            ekill=p;

        p = p->next;
    }

    if (!ekill)
    {
        printf("nonc'è\n");
        sem_post(&s->lsem);
        return 0;
    }

    p = l->head;

    while (p)
    {
        c=(clientSlot *)p->p;

        if (c->selected == false)
        {
            if (c->sock == 0)
                n++;

            if (n > maxfree && n > POOL_SIZE)
            {
                delete_from(s->clientlist, ekill);
                sem_post(&s->lsem);

                return 1;
            }
        }
        p = p->next;
    }

    sem_post(&s->lsem);

    return 0;
}

static void *findSlot(serverConfig *s)
{
    list *l;
    elem* p;
    clientSlot *c = NULL;

    sem_wait(&s->lsem);

    l = s->clientlist;
    p = l->head;

    while (p)
    {
        c=(clientSlot *)p->p;
        if (c)
        {
            if (c->sock == 0)
            {
                c->selected=true;
                break;
            }
            c=NULL;
        }
        p = p->next;
    }

    sem_post(&s->lsem);

    return c;
}

void serverLoop(serverConfig *s)
{
    unsigned int tmp_sock;
    struct sockaddr_in tmp_addr;
    int len=sizeof(tmp_addr);
    clientSlot *c;

    while (1)
    {
        if (countSlots(s,0) <= MAX_POOL_SIZE)
        {
            if ((tmp_sock = accept(s->sock, (struct sockaddr *) &tmp_addr, \
                                  (socklen_t*) &len)) >= 0)
            {

                while ( (c = findSlot(s)) == 0 )
                    initThread(s);

                c->sock=tmp_sock;
                c->caddr=tmp_addr;
                sem_post(&c->sem);
            }
            else
                my_perror("accept()",0);

            while (countSlots(s, 0) < MAX_FREE)
                initThread(s);
        }
        usleep(10);
        fflush(stdout);
    }

}

void serverSetup(serverConfig *s)
{
    int on = 1,ret;

    s->clientlist = init_list();

    if ((s->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        my_perror("socket()",1);

    if ((ret = setsockopt(s->sock, SOL_SOCKET, SO_REUSEADDR, &on, \
                          sizeof(on))) < 0)
        my_perror("setsockopt()",1);

    memset(&s->my_addr, 0, sizeof(s->my_addr));
    s->my_addr.sin_family = AF_INET;
    s->my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s->my_addr.sin_port = htons(s->port);

    if ((ret = bind(s->sock, (struct sockaddr*)&s->my_addr, \
                    sizeof(s->my_addr))) < 0)
        my_perror("bind()",1);

    if ((ret = listen(s->sock, 0)) < 0)
        my_perror("listen()",1);

    printf("* listening for connections on port %u\n", s->port);
}

