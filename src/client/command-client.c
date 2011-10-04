#include "command-client.h"

static command handled_cmd[] =
{
{CMD_HELP,                 "help",                 0, help},
{CMD_QUIT,                 "quit",                 0, quit},
{CMD_CONFIGURE_COMPRESSOR, "configure-compressor", 1, configureCompressor},
{CMD_CONFIGURE_NAME,       "configure-name",       1, configureName},
{CMD_CONFIGURE_CRYPT,      "configure-crypt",      0, dummy},
{CMD_SHOW_CONFIGURATION,   "show-configuration",   0, showConfiguration},
{CMD_SEND,                 "send",                 1, upload},
{CMD_COMPRESS,             "compress",             1, compress},
{CMD_PING,                 "ping",                 0, ping},
{CMD_REMOVE,               "remove",               1, removefile},
{CMD_LIST,                 "list",                 0, listfiles},
{CMD_CONNECT,              "connect",              0, connectSrv},
{CMD_VOID,                 NULL,                   0, dummy }

};

void commandLoop(clientConfig *c)
{
    char buffer[MSGSIZE+1], name[MSGSIZE], param[MSGSIZE];

    printf("\n");

    while (1)
    {
        printf("> ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);

        if (strlen(buffer) > 0 && buffer[strlen(buffer)-1] == '\n')
            buffer[strlen(buffer)-1] = '\0';

        if (strlen(buffer) < MSGSIZE)
        {
            strcpy(name,"");
            strcpy(param,"");
            sscanf(buffer, "%s %[^\t\n]", name, param);
            cleanTailSpaces(param);

            if (!strlen(param))
                executeCommand(name, NULL, c);
            else
                executeCommand(name, param, c);
        }
        else
            printf("Command is too long can't parse this.\n");

        printf("\n");
    }
}

bool executeCommand(char *name, char *param, clientConfig *c)
{
    command *cmd=getCommandFromName(name, handled_cmd);

    if (cmd && c && checkCommand(cmd, param) && cmd->h)
            return (*cmd->h)(param,(void *)c);

    return false;
}
