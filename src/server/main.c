#include "command-server.h"
#include "socket-server.h"
#include "thread.h"

void loadArgs(int argc, char** argv, serverConfig *conf)
{
    conf->port=9000;

    if (argc != 1 || argc !=2)
        printf("usage: %s <port>\n\n",argv[0]);

    if (argc == 2)   
        conf->port=atoi(argv[1]);
}

void printConfig(serverConfig *conf)
{
    printf("port : %d\n",conf->port);
    printf("uid  : %d\n",getuid());

    printf("\n");
}

void welcome()
{
    printf("YARC - yet another remote compressor\n");
    printf("server version %s\n------------------------------\n", PACKAGE_VERSION);
}

int main(int argc,char** argv)
{
    serverConfig s;

    welcome();

    loadArgs(argc,argv,&s);
    printConfig(&s);

    if (getuid()!=0 && s.port<=1024)
    {
        printf("root privileges required: using port <=1024\n");
        exit(1);
    }
    errno=0;

    /* init */
    serverSetup(&s);
    initPool(&s);
    serverLoop(&s);

    /* shutdown */
    close(s.sock);
    free_list(s.clientlist);

    return 0;
}
