#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "utility.h"

#define HLEN 3
#define MSGSIZE 1024
#define HAS_PARAM 1
#define NO_PARAM  0


typedef enum commandtype
{
    CMD_VOID=0,
    CMD_QUIT,
    CMD_HELP,
    CMD_CONFIGURE_COMPRESSOR,
    CMD_CONFIGURE_NAME,
    CMD_CONFIGURE_CRYPT,
    CMD_SHOW_CONFIGURATION,
    CMD_SEND,
    CMD_COMPRESS,
    CMD_REMOVE,
    CMD_LIST,
    CMD_CONNECT,
    CMD_PING,
    CMD_ACK,
    CMD_ABORT,
    CMD_FILE_INFO
} commandtype;

typedef bool(*handler)(char *, void *);

typedef struct command
{
    commandtype type;	
    char        *name;
    bool        needparam;
    handler     h;
} command;

typedef struct command_err
{
    commandtype type;	
    char *err;
} command_err;

bool sendCommand(int, char *, unsigned int *);
int  recvCommand(char *, unsigned int *);
char *packetForge(int, const char *);

command *getCommandFromName(char *, command *);
command *getCommandFromType(commandtype, command *);

bool checkCommand(command *, char *);

#endif
