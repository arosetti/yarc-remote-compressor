#ifndef FILE_H
#define FILE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "sha1.h"
#include "utility.h"

int  fileSize(const char *);
bool fileExists (const char *);
int  isDir(const char *);
bool isDirEmpty(const char *);
void addSlash(char *);
void deltree(const char *);
char *listFiles(const char *);
void makeSubDir(char *);
char *sha1Digest(const char *);
void printFileInfo(const char *,int ,const char *);

#endif
