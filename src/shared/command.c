#include "command.h"

command_err handled_err[] =
{
    {CMD_CONFIGURE_COMPRESSOR , "specify a valid compression algorithm type"},
    {CMD_CONFIGURE_NAME,        "specify remote archive file name" },
    {CMD_SEND,                  "specify file or directory name"},
    {CMD_COMPRESS,              "specify destination directory of the archive"},
    {CMD_REMOVE,                "specify file name to remove"},
    {CMD_VOID,                  NULL}
};

bool sendCommand(int type, char *param, unsigned int *sock)
{
    char *p = packetForge(type,param);
    int ret, len = HLEN;

    if (param)
        len+=strlen(param);

    if ( (ret = send(*sock, p, len, 0)) < 0 )
    {
        my_perror("send()",0);
        free(p);
        close(*sock);
        return false;
    }

    free(p);

    if (ret != len)
        return false;

    return true;
}

int recvCommand(char *param, unsigned int *sock)
{
    int type = 0, ret = 0;
    char header[HLEN];
    char *buffer;
    unsigned short length = 0;

    header[0]=0;

    if ( (ret=recv(*sock, header, HLEN, 0)) < 0 )
    {
        my_perror("recv()",0);
        close(*sock);
        return 0;
    }

    type=header[0];

    length |= (unsigned short)header[1];
    length  = length<<8;
    length |= (unsigned short)header[2];

    if ((type > 0) && (length > 0) && (length < MSGSIZE))
    {
        buffer = my_malloc(sizeof(char)*length+1);

        if ( (ret=recv(*sock, buffer, length, 0)) < 0 )
        {
            my_perror("recv()",0);
            close(*sock);
            free(buffer);
            return 0;
        }
        buffer[ret]=0;

        if (ret == length)
            if (param && (strlen(buffer) > 0))
            {
                strncpy(param, buffer ,length);
                param[length]=0;
            }
        free(buffer);
    }

    return type;
}

char *packetForge(int type, const char *param)
{
    char *packet;
    unsigned short plen = 0, temp;

    if (param)
        plen = strlen(param);

    packet = my_malloc(sizeof(char)*(plen + HLEN + 1));

    packet[0] = (unsigned char) type;

    temp=plen;
    temp &= 0x00ff;
    packet[2] = (char)temp;
    temp = plen;
    temp = temp>>8;
    packet[1] = (char)temp;
    packet[3] = 0;

    if (param)
        strcpy(packet+3, param);

    return packet;
}

command *getCommandFromName(char *name, command *cs)
{
	int i;
    for( i=0; cs[i].type != CMD_VOID; i++)
    {
        if (strcmp(cs[i].name,name) == 0)
            return &cs[i];
    }
    return &cs[i];
}

command *getCommandFromType(commandtype type, command *cs)
{
	int i;
    for( i=0; cs[i].type != CMD_VOID; i++)
    {
        if (cs[i].type == type)
            return &cs[i];
    }
    return &cs[i];
}

static command_err *searchErr(commandtype type)
{
    int i;

    for(i=0;handled_err[i].type != CMD_VOID; i++)
    {
        if (handled_err[i].type == type)
            return &handled_err[i];
    }

    return 0;
}

bool checkCommand(command *cmd, char *param)
{
    command_err *c_err;

    if (!cmd)
        return false;

    if (cmd->type == CMD_VOID)
    {
        printf("Command not found!\n");
        return false;
    }

    if (!cmd->needparam && param != NULL)
    {
        printf("No param needed\n");
        return false;
    }

    if (cmd->needparam && param == NULL)
    {
        c_err = searchErr(cmd->type);

        if (c_err && c_err->err)
            printf("%s\n",c_err->err);
        else
            printf("unknown parameter error\n");
        return false;
    }

    return true;
}

