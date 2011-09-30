#include "function-client.h"

bool help(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;

    printf("show-configuration           : shows remote configuration\n");
    printf("configure-name        <name> : set archive name\n");
    printf("configure-compressor  <type> : set compression algorithm\n");
    printf("compress              <path> : create archive to specified path\n");
    printf("send                  <file> : send file to server\n");
    printf("remove                <file> : remove uploaded file from server\n");
    printf("list                         : list uploaded files\n");
    printf("ping                         : ping server\n");
    printf("connect                      : connect to server\n");
    printf("quit                         : exit from program\n\n");
    printf("available compressors: none gnuzip bzip2 xz zip\n");

    sendCommand(CMD_HELP, NULL, &(c->sock));
    return true;
}

bool configureCompressor(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    sendCommand(CMD_CONFIGURE_COMPRESSOR, (char*)param, &(c->sock));

    if (recvCommand(0,&(c->sock)) == CMD_ACK)
        printf("* acknowledged compression algorithm type: %s\n", param);
    else
    {
        printf("* rejected compression algorithm type: %s\n", param);
        return false;
    }

    return true;
}

bool configureName(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;

    if( strlen(param) > 255 )
    {
        printf("* archive name is too long\n");
            return false;
    }

    if(strchr(param, ' ') != NULL)
    {
        printf("* sorry, archive name can't use spaces!\n");
        return false;
    }

    sendCommand(CMD_CONFIGURE_NAME, (char*)param, &(c->sock));

    if (recvCommand(0, &(c->sock)) == CMD_ACK)
        printf("* acknowledged archive name: %s\n",param);
    else
    {
        printf("* rejected archive name: %s\n",param);
        return false;
    }

    return true;
}

bool showConfiguration(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    char buffer[MSGSIZE];
    char name[256];
    char type[16];

    sendCommand(CMD_SHOW_CONFIGURATION, NULL, &(c->sock));

    if (recvCommand(buffer, &(c->sock)) == CMD_SHOW_CONFIGURATION )
    {
        sscanf(buffer,"%s / %s",name,type);
        printf("* archive name: %s\n",name);
        printf("* archive type: %s\n",type);
    }
    else
        return false;

    return true;
}

bool upload(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    if (!isDir(param))
    {
        sendCommand(CMD_SEND, (char *)param, &(c->sock));
        return pushfile(param, &(c->sock));
    }
    else
    {
        printf("* parameter %s is a directory, sending recursively\n",param);
        senddir(param,&(c->sock));
    }

    return true;
}

bool compress(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    int i=0;
    char buffer[MSGSIZE];
    char archive[MSGSIZE];

    /* direxists -> makesubdir*/

    for(i=0;i<MSGSIZE;i++)
        buffer[i]=0;

    sendCommand(CMD_COMPRESS, NULL, &(c->sock));

    if (recvCommand(buffer, &(c->sock)) != CMD_SEND)
    {
        printf("* compress \"%s\" failed. no file sent or server error\n",param);

        return false;
    }

    sendCommand(CMD_ACK, NULL, &(c->sock));
    printf("* downloading remote archive \"%s\"\n",buffer);

    pullfile(buffer, &(c->sock));

    strcpy(archive,param);
    addSlash(archive);
    strcat(archive,buffer);
    printf("* moving file \"%s\" to %s\n",buffer,archive);
    if (rename(buffer,archive)<0)
        my_perror("rename()",0);

    return true;
}

bool ping(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    sendCommand(CMD_PING, NULL , &(c->sock));

    if ( recvCommand(0, &(c->sock)) == CMD_PING)
        printf("* pong!\n");
    else
    {
        printf("* ping failed\n");
        return false;
    }

    return true;
}

bool removefile(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    sendCommand(CMD_REMOVE, (char *)param, &(c->sock));

    if (recvCommand(0, &(c->sock)) == CMD_ACK)
        printf("* %s removed\n",param);
    else
    {
        printf("* can't delete \"%s\"\n",param);
        return false;
    }
    return true;
}

bool listfiles(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    char buffer[MSGSIZE];

    sendCommand(CMD_LIST, NULL , &(c->sock));

    if (recvCommand(buffer, &(c->sock)) == CMD_ACK)
        printf("* uploaded files:\n%s\n",buffer);
    else
        printf("* remote folder is empty!\n");

    return true;
}

bool quit(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    if (close(c->sock) < 0)
        my_perror("close()",1);
    else
        printf("* socket closed, bye...\n");

    exit(0);
    return false;
}

bool connectSrv(char *param, void *conf)
{
    clientConfig *c=(clientConfig*)conf;
    clientConnect(c);
    return true;
}

bool dummy(char *param, void *conf)
{
    return true;
}
