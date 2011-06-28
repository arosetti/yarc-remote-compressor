#include "command-server.h"

command handled_cmd[] = 
{
    {CMD_HELP,                 "help",                 0, dummy,},
    {CMD_CONFIGURE_COMPRESSOR, "configure-compressor", 1, configureCompressor},
    {CMD_CONFIGURE_NAME,       "configure-name",       1, configureName},
    {CMD_SHOW_CONFIGURATION,   "show-configuration",   0, showConfiguration},
    {CMD_SEND,                 "send",                 1, download},
    {CMD_COMPRESS,             "compress",             0, compress},
    {CMD_PING,                 "ping",                 0, pong},
    {CMD_REMOVE,               "remove",               1, removefile},
    {CMD_LIST,                 "list",                 0, listfiles},
    {CMD_VOID,                 NULL,                   0, dummy}
};

static void executionInfo(const char *name, clientSlot *c)
{
    printf("* <%s> executed from %s:%d\n\n",name,c->host,c->port);
}

bool executeCommand(commandtype type, char *param, clientSlot *c)
{
    command *cmd=getCommandFromType(type, handled_cmd);

    if(cmd && c && checkCommand(cmd, param) && cmd->h)
    {
            executionInfo(cmd->name,c);
            return (*cmd->h)(param, (void *)c);
    }

    return false;
}
