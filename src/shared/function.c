#include "function.h"
#include "command.h"

bool senddir(const char *dir, unsigned int *sock)
{ 
    struct dirent *pdir;
    struct stat st;
    DIR *d=opendir(dir);
    char *path=(char *)my_malloc(sizeof(char)*256);
    char *buffer=(char *)my_malloc(sizeof(char)*256); 


    if(!path)
        return false;

    while((pdir=readdir(d))!=NULL) 
    {
        if(strcmp(pdir->d_name,".") == 0)
            continue;
        if(strcmp(pdir->d_name,"..") == 0)
            continue;

        strcpy(path, dir);
        addSlash(path);
        strcat(path, pdir->d_name);
        if (!(lstat(path, &st)>=0)) 
            continue;
        if (st.st_mode&S_IFREG)
        {
            sendCommand(CMD_SEND,(char *)path, sock);
            pushfile(path, sock);
        }
        else if (st.st_mode&S_IFDIR)
            senddir(path, sock);        
    }   
  
    free(buffer);
    free(path);
    return true;
}

bool pushfile(const char *filename, unsigned int *sock) 
{ 
    char buffer[BLOCK], msg[256] = "";   
    int  size = fileSize(filename);
    int  nread, nwrite, tot=size; 
    int  file;
    char *sha = shaDigest(filename);

    int columns=0;

    struct timeval start;
    struct timeval stop;
    double difftime=0;

    if( (file = open(filename, O_RDONLY)) < 0)
    {
        my_perror("open()",0);
        sendCommand(CMD_ABORT, NULL, sock);
        free(sha);
        return false;
    }

    printFileInfo(filename,size,sha);
    sprintf(msg,"filesize:%d sha:%s",size,sha);
    sendCommand(CMD_FILE_INFO, (char *)msg, sock);

    if(recvCommand(NULL, sock) != CMD_ACK)
    {
        free(sha);
        return false;
    }

    if(size<=0)
    {
        free(sha);
        return false;
    }

    gettimeofday(&start,NULL);

    while(1) 
    {
	    memset(buffer, 0, BLOCK); 
	    if ((nread=read(file, buffer, BLOCK)) < 0 )
        {
		    my_perror("read()",0);
            return false;
		}
        if ((nwrite=write(*sock, buffer, nread)) < 0) 
		    my_perror("write()",0);
	    tot-=nwrite;

        if(columns > 0)
            stepProgressBar(size-tot,columns,size);

	    if (tot <= 0) 
		    break; 
    }
    gettimeofday(&stop,NULL);

    if(recvCommand(NULL, sock) != CMD_ACK)
        printf("* send of \"%s\" failed\n",filename);
    else
    {
        difftime=timeval_diff(&stop,&start);

        printf("* time:     %f seconds, ",difftime);
        num2human((long double)(size/difftime));
        printf("B/s\n");

        printf("* result:   sending \"%s\" completed successfully\n",filename);
    }

    free(sha);
    close(file);
    return true;
}

bool pullfile(const char *filename, unsigned int *sock) 
{
    char buffer[BLOCK];   
    int size, nread=0, nwrite=0, tot=0, file;  
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    char *sha=NULL, recv_sha[41] = "";

    int columns=0;
   
    struct timeval start;
    struct timeval stop;
    double difftime=0;


    if((file=open(filename, O_WRONLY | O_CREAT, mode)) < 0)
    {
        makeSubDir((char*)filename);
        remove(filename);
        if((file=open(filename, O_WRONLY | O_CREAT, mode))<0)
        {
            my_perror("open()",0);
            return false;
        }
    }

    if(recvCommand(buffer, sock) != CMD_FILE_INFO)
    {
        printf("* Can't get \"%s\". Permission or I/O error \n\n",filename);
        close(file);
        return false;
    }

    sscanf(buffer,"filesize:%d sha:%s",&size,recv_sha);
    printFileInfo(filename,size,recv_sha);

    sendCommand(CMD_ACK, NULL, sock);

    if(size==0)
    {
        close(file);
        return true;
    }
    if(size<0)
    {
        if(remove(filename) < 0)
            my_perror( "remove()",0);
        close(file);
        return false;
    }

    gettimeofday(&start,NULL);

    while(1) 
    { 
	    memset(buffer, 0, BLOCK); 
	    if ((nread=read(*sock, buffer, BLOCK)) < 0) 
        {
		    my_perror("read()",0); 
            return false;
        }	    
        if ((nwrite=write(file, buffer, nread)) < 0 ) 
		    my_perror(" write()",0); 
	    tot+=nread;

        if(columns > 0)
            stepProgressBar(tot,10,size);

	    if (tot == size) 
		    break;  
    }

    gettimeofday(&stop,NULL);
    difftime=timeval_diff(&stop,&start);

    printf("* time:     %f seconds, ",difftime);
    num2human((long double)(tot/difftime));
    printf("B/s\n");

    sha=shaDigest(filename);
    if(strcmp(sha,recv_sha) == 0)
    {
        printf("* result:   digest of %s is correct\n",filename);
        sendCommand(CMD_ACK, NULL, sock);
    }
    else
    {
        printf("* result:   digest of %s is wrong\n",filename);
        sendCommand(CMD_ABORT, NULL, sock);
    }

    printf("\n");

    if(sha)
        free(sha);
        
    close(file);
    return true;
}
