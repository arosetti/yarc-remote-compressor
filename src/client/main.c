#include "socket-client.h"
#include "command-client.h"
#include "../shared/utility.h"

void loadArgs(int argc, char** argv, clientConfig *conf)
{
    strcpy(conf->host,"127.0.0.1");
    conf->port=9000;

    if (argc != 1 || argc !=3)
        printf("usage: %s <host> <port>\n\n",argv[0]);

    if (argc == 3)
    {
        strcpy((char*)conf->host,(char*)argv[1]);
        conf->port=atoi(argv[2]);
    }
}

void printConfig(clientConfig *conf)
{
    printf("host : %s\n",conf->host);
    printf("port : %d\n",conf->port);
    printf("uid  : %d\n",getuid());

    printf("\n");
}

void welcome()
{
    printf("YARC - yet another remote compressor\n");
    printf("client version %s\n------------------------------\n", PACKAGE_VERSION);
    printf("type \"help\" to see command list\n");
    printf("max message length is %d\n\n",MSGSIZE);
}

int main(int argc,char** argv)
{
    clientConfig c;
    /*signal(SIGINT, sigintHandler);*/
    welcome();

    loadArgs(argc,argv,&c);
    printConfig(&c);
    errno=0;

    clientSetup(&c);
    clientConnect(&c);
    commandLoop(&c);

    close(c.sock);

    return 0;
}
