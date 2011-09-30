#include "command-client.h"

static command handled_cmd[] = 
{
{CMD_HELP,                 "help",                 0, help},
{CMD_QUIT,                 "quit",                 0, quit},
{CMD_CONFIGURE_COMPRESSOR, "configure-compressor", 1, configureCompressor},
{CMD_CONFIGURE_NAME,       "configure-name",       1, configureName},
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
    char buffer[MSGSIZE+1];
    char *name=0,*param=0;

    printf("\n");

    while(1)
    {
        strcpy(buffer,"");  
        printf("> ");
        scanf("%1024[^\t\n]",buffer);
        if (strlen(buffer) < MSGSIZE)
        {
            name = (char*) my_malloc(sizeof(char) * MSGSIZE);
            param = (char*) my_malloc(sizeof(char) * MSGSIZE);

            strcpy(name,"");
            strcpy(param,"");  
            sscanf(buffer,"%s %[^\t\n]",name,param);
            cleanTailSpaces(param);

            if (strlen(param) == 0) 
            {
                free(param);
                param=NULL;
            }
            executeCommand(name,param,c);
        }
        else
            printf("Command is too long can't parse this.\n");

        printf("\n");

        if (name)
           free(name);
        if (param)
           free(param);

        while(!getchar());
    }
}

bool executeCommand(char *name, char *param, clientConfig *c)
{
    command *cmd=getCommandFromName(name, handled_cmd);

    if(cmd && c && checkCommand(cmd, param) && cmd->h)
            return (*cmd->h)(param,(void *)c);

    return false;
}
