#include "socket-client.h"
#include "command-client.h"
#include "../shared/utility.h"

void helpArgs()
{
    printf("help:\n");
    printf("-h                  :  help\n");
    printf("-s <ip>             :  server ip\n");
    printf("-p <port>           :  server port\n");
    printf("\n");
}

void printConfig(clientConfig *conf)
{
    printf("host : %s\n",conf->host);
    printf("port : %d\n",conf->port);
    printf("uid  : %d\n",getuid());
    printf("\n");
}

int loadArgs(int argc, char **argv, clientConfig *conf)
{
    int opt, tmp, index;
    opterr = 0;

    /*default*/
    strcpy(conf->host,"127.0.0.1");
    conf->port=9000;

    while ((opt = getopt (argc, argv, "hs:p:d")) != -1)
        switch (opt)
        {
            case 'h':
                helpArgs();
                exit(0);
            break;
            case 'd':
            break;
            case 's':
                if (!isValidIp(optarg))
                {
                    fprintf(stderr, "%s is not a valid ip address!\n", optarg);
                    exit(1);
                }
                strcpy(conf->host, optarg);
            break;
            case 'p':
                tmp = atoi(optarg);
                if (tmp >= 0 || tmp <= 65535)
                    conf->port = tmp;
                else
                    fprintf(stderr, "wrong -p parameter. you must use a valid port number\n");
            break;
            case '?':
                if (optopt == 's')
                fprintf (stderr, "option -%c requires an argument.\n\n", optopt);
                else if (optopt == 'p')
                fprintf (stderr, "option -%c requires an argument.\n\n", optopt);
                else if (isprint (optopt))
                fprintf (stderr, "unknown option `-%c'.\n\n", optopt);
                else
                fprintf (stderr,
                        "unknown option character `\\x%x'.\n\n",
                        optopt);
                break;
            default:
                break;
        }    

    for (index = optind; index < argc; index++)
        fprintf(stderr, "non-option argument %s\n", argv[index]);

    return 0;
}

int main(int argc,char** argv)
{
    clientConfig conf;

    printf("YARC - yet another remote compressor\n");
    printf("client version %s\n------------------------------\n", PACKAGE_VERSION);
    printf("type \"help\" to see command list\n");
    printf("max message length is %d\n\n",MSGSIZE);

    loadArgs(argc, argv, &conf);
    printConfig(&conf);

    clientSetup(&conf);
    clientConnect(&conf);
    commandLoop(&conf);

    close(conf.sock);

    return 0;
}
