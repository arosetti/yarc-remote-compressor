#!/bin/sh

make clean
echo "#define VERSION \"$1\"" > shared/version.h
tar cvzf remote-compressor-$1.tar.gz *
#install cloc
cloc --extract-with='gzip -dc >FILE< | tar xf -' remote-compressor-$1.tar.gz
cd ..

