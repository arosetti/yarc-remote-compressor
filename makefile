CC=gcc
CFLAGS=-c -g -Wall -Werror -O2 -pedantic

LDC= 
LDS=-lpthread -larchive -lz -lbz2 

OBJCLI= client/command-client.o client/client.o client/function-client.o
OBJSRV= server/command-server.o server/server.o server/function-server.o \
        server/archive.o  server/thread.o server/list.o
OBJSHR= shared/utility.o  shared/function.o  shared/sha1.o shared/file.o shared/time.o shared/command.o


all: client server


client:    c_main.o $(OBJCLI) $(OBJSHR)
	$(CC) $(LDC) -o client/$@ client/main.o $(OBJCLI) $(OBJSHR)

server:    s_main.o $(OBJSRV) $(OBJSHR) 
	$(CC) $(LDS) -o server/$@ server/main.o $(OBJSRV) $(OBJSHR)



c_main.o: client/main.c
	      $(CC) $(CFLAGS) $< -o client/main.o
command-client.o:  client/command-client.c 
	      $(CC) $(CFLAGS) $< -o client/command-client.o
client.o:  client/client.c 
	      $(CC) $(CFLAGS) $< -o client/client.o
function-client.o: client/function-client.c
	      $(CC) $(CFLAGS) $< -o client/function-client.o




s_main.o: server/main.c
	      $(CC) $(CFLAGS) $< -o server/main.o
command-server.o:  server/command-server.c 
	      $(CC) $(CFLAGS) $< -o server/command-server.o
server.o:  server/server.c 
	      $(CC) $(CFLAGS) $< -o server/server.o
function-server.o: server/function-server.c
	      $(CC) $(CFLAGS) $< -o server/function-server.o
archive.o: server/archive.c
	      $(CC) $(CFLAGS) $< -o server/archive.o
thread.o: server/thread.c
	      $(CC) $(CFLAGS) $< -o server/thread.o
list.o:   server/list.c
	      $(CC) $(CFLAGS) $< -o server/list.o




command.o:  shared/command.c
	      $(CC) $(CFLAGS) $< -o shared/command.o
function.o:  shared/function.c
	      $(CC) $(CFLAGS) $< -o shared/function.o
time.o:    shared/time.c
	      $(CC) $(CFLAGS) $< -o shared/time.o
file.o:    shared/file.c
	      $(CC) $(CFLAGS) $< -o shared/file.o
sha1.o:    shared/sha1.c
	      $(CC) $(CFLAGS) $< -o shared/sha1.o
utility.o: shared/utility.c
	      $(CC) $(CFLAGS) $< -o shared/utility.o




clean:  
	      @rm -f  ./server/*.o ./client/*.o ./shared/*.o ./server/server ./client/client

tar:      clean
	      tar czvf yarc.tgz client/ server/ shared/ makefile build.sh new-release.sh

