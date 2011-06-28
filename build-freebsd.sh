#!/bin/sh
# build.sh ver 0.1

make_object()
{
    echo "gcc -c -g -Wall -Werror -O2 -pedantic $2.c -o $2.o"
    gcc -c -g -Wall -Werror -O2 -pedantic $1/$2.c -o $1/$2.o
}

rm -f  server/*.o client/*.o shared/*.o \
       server/server client/client  \
       > /dev/null

make_object "client" "main"
make_object "client" "command-client"
make_object "client" "client"
make_object "client" "function-client"

make_object "server" "main"
make_object "server" "command-server"
make_object "server" "server"
make_object "server" "function-server"
make_object "server" "archive"
make_object "server" "thread"
make_object "server" "list"

make_object "shared" "command"
make_object "shared" "time"
make_object "shared" "sha1"
make_object "shared" "file"
make_object "shared" "function"
make_object "shared" "utility"

cd client
gcc -o client main.o command-client.o client.o function-client.o \
       ../shared/function.o ../shared/file.o ../shared/utility.o ../shared/time.o ../shared/sha1.o ../shared/command.o

cd ../server
gcc -o server main.o command-server.o server.o function-server.o list.o archive.o thread.o \
       ../shared/function.o ../shared/file.o ../shared/utility.o ../shared/time.o ../shared/sha1.o ../shared/command.o \
      -lpthread -lz -larchive -lbz2

cd ..
