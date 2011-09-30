#include "function-server.h"

const char *arctypes[] = {"none", "bzip2", "gnuzip", "xz", "zip", 0};

bool configureCompressor(char *param, void *conf)
{
    clientSlot *c = (clientSlot *) conf;
    int i=0;

    if (strlen(param) < 16)
        while(arctypes[i])
        {
            if (strcmp((char*)param,(char*)arctypes[i]) == 0)
            {
                sendCommand(CMD_ACK, NULL, &(c->sock));
                strcpy(c->arctype,param);
                printf("* client compression algorithm type acknowledged!\n\n");
                return true;
            }
            i++;
        }

    sendCommand(CMD_ABORT, NULL, &(c->sock));
    printf("* client compression algorithm type is wrong!\n\n");

    return false;
}

bool configureName(char *param, void *conf)
{
    clientSlot *c = (clientSlot *) conf;

    if (strlen(param) > 255)
    {
        sendCommand(CMD_ABORT, NULL, &(c->sock));
        printf("* client archive name is too long!\n\n");

        return false;
    }

    if (strchr(param, ' ') != NULL)
    {
        printf("* sorry, archive name can't use spaces!\n");
        return false;
    }

    strcpy(c->arcname,param);

    sendCommand(CMD_ACK, NULL, &(c->sock));
    printf("* client archive name acknowledged!\n\n");

    return true;
}

bool showConfiguration(char *param, void *conf)
{
    clientSlot *c = (clientSlot *) conf;
    char buffer[MSGSIZE];

    sprintf(buffer,"%s / %s",c->arcname,c->arctype);
    sendCommand(CMD_SHOW_CONFIGURATION, buffer, &(c->sock));

    return true;
}

bool download(char *param, void *conf)
{
    clientSlot *c = (clientSlot *) conf;
    char buffer[MSGSIZE];

    if (param[0] == '/')
        param++;

    strcpy(buffer,c->dir);
    addSlash(buffer);
    strcat(buffer,param);

    return pullfile(buffer,&(c->sock));
}

bool compress(char *param, void *conf)
{
    clientSlot *c = (clientSlot *) conf;
    archiveData *a=NULL;
    bool added=false;
    char buffer[MSGSIZE];

    strcpy(buffer,c->dir);
    if (buffer[strlen(buffer)-1] != '/')
        strcat(buffer,"/.");

    if (strcmp(c->arctype,"none") == 0)
        a=init_archive(c->arcname, none);
    else if (strcmp(c->arctype,"gnuzip") == 0)
        a=init_archive(c->arcname, gnuzip);
    else if (strcmp(c->arctype,"bzip2") == 0)
        a=init_archive(c->arcname, bzip2);
    else if (strcmp(c->arctype,"xz") == 0)
        a=init_archive(c->arcname, xz);
    else if (strcmp(c->arctype,"zip") == 0)
        a=init_archive(c->arcname, zip);
    else
    {
        printf("* compression algorithm type \"%s\" not known\n",c->arctype);
        sendCommand(CMD_ABORT, NULL, &(c->sock));
        free(a->name);
        free(a);

        return false;
    }

    added=add_dir(buffer,a);
    finalize_archive(a);

    if (!added)
    {
        printf("* archive is empty!\n");
        sendCommand(CMD_ABORT, NULL, &(c->sock));
        remove(a->name);
        free(a->name);
        free(a);

        return false;
    }
    else
    {
        printf("* archive \"%s\" created\n", a->name);
        sendCommand(CMD_SEND, a->name, &(c->sock));
    }

    if (recvCommand(NULL, &(c->sock)) == CMD_ACK)
    {
        pushfile(a->name,&(c->sock));
    }

    remove(a->name);
    deltree(c->dir);
    mkdir(c->dir,0775);

    free(a->name);
    free(a);

    printf("\n");

    return true;
}

bool pong(char *param, void *conf)
{
    clientSlot *c = (clientSlot *) conf;
    sendCommand(CMD_PING, NULL, &(c->sock));
    printf("* sending ping reply\n\n");

    return true;
}

bool removefile(char *param, void *conf)
{
    clientSlot *c = (clientSlot *) conf;
    char *buffer = (char*) my_malloc(sizeof(char)*(strlen(c->dir) + \
                   strlen(param) + sizeof(char)*2));

    strcpy(buffer,c->dir);
    addSlash(buffer);
    strcat(buffer,param);

    if (remove(buffer) < 0)
    {
        printf("* can't delete \"%s\" file\n",param);
        sendCommand(CMD_ABORT, NULL, &(c->sock));
    }
    else
    {
        printf("* file \"%s\" removed\n\n",param);
        sendCommand(CMD_ACK, NULL, &(c->sock));
    }

    free(buffer);

    return 1;
}

bool listfiles(char *param, void *conf)
{
    clientSlot *c = (clientSlot *) conf;
    char *buffer=listFiles(c->dir);

    if (strlen(buffer) == 0)
        sendCommand(CMD_ABORT, NULL, &(c->sock));
    else
    sendCommand(CMD_ACK, buffer , &(c->sock));

    if (buffer)
        free(buffer);

    return true;
}

bool dummy(char *param, void *conf)
{
    return true;
}
