#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <archive.h>
#include <archive_entry.h>
#include <dirent.h>
#include <limits.h>

#ifdef linux
    #include <malloc.h>
#endif

#include "../shared/file.h"

#define ABLOCK 1024

typedef struct archive archive;
typedef struct archive_entry archive_entry;

typedef enum arctype
{
    none,gnuzip,bzip2,xz,zip
} arctype;

typedef struct archiveData
{
    char *name;
    arctype type;
    archive *arc;
    int fd;
} archiveData;

archiveData *init_archive(char *, arctype);
bool         finalize_archive(archiveData *);
int  add_file(const char *, archiveData *);
int  add_dir(const char *, archiveData *);


#endif
