#include "archive.h"

archiveData *init_archive(char *name, arctype type)
{
    archiveData *a = (archiveData *)malloc(sizeof(archiveData));
    size_t namesize = strlen(name) + 1;

    if (!name)
        return 0;

    a->arc = archive_write_new();

    a->name=(char *)malloc(namesize);
    memset(a->name, '\0', namesize);

    if (type == none)
    {
        sprintf(a->name, "%s.tar", name);
        if (archive_write_set_compression_none(a->arc) < 0)
              fprintf(stderr,"can't set compression : %s\n", \
              archive_error_string(a->arc));
    }
    else if (type == gnuzip)
    {
        sprintf(a->name, "%s.tar.gz", name);
        if (archive_write_set_compression_gzip(a->arc) < 0)
              fprintf(stderr,"can't set compression : %s\n", \
              archive_error_string(a->arc));
    }
    else if (type == bzip2)
    {
        sprintf(a->name, "%s.tar.bz2", name);
        if (archive_write_set_compression_bzip2(a->arc) < 0)
            fprintf(stderr,"can't set compression : %s\n", \
            archive_error_string(a->arc));
    }
#ifdef linux
    else if (type == zip)
    {
        sprintf(a->name, "%s.zip", name);
        if (archive_write_set_format_zip(a->arc) < 0)
            fprintf(stderr,"can't set compression : %s\n", \
            archive_error_string(a->arc));
    }

    else if (type == xz)
    {
        sprintf(a->name, "%s.tar.xz", name);
        if (archive_write_set_compression_xz(a->arc) < 0)
            fprintf(stderr,"can't set compression : %s\n", \
            archive_error_string(a->arc));
    }
#endif
    else
        fprintf(stderr, "compression type not known\n");

    /*if (archive_write_set_format_ustar(a->arc) < 0)
         printf("can't set format : %s\n" , archive_error_string(a->arc));*/

    if (archive_write_set_format_pax_restricted(a->arc) < 0)
        printf("can't set format : %s\n" , archive_error_string(a->arc));


    if (fileExists(a->name))
    {
        printf("* file \"%s\" already exists, overwriting...\n", a->name);
        if (remove(a->name) < 0)
            printf("* can't delete \"%s\"\n",a->name);
    }

    if (archive_write_open_filename(a->arc, a->name) < 0)
        printf("error opening archive : %s\n", archive_error_string(a->arc));

    return a;

}

bool finalize_archive(archiveData *a)
{
    if (!a)
        return false;

    if (archive_write_close(a->arc) < 0)
    {
        fprintf(stderr,"Errore archive close : %s\n" ,
                archive_error_string(a->arc));
        return false;
    }

#ifdef linux
    if (archive_write_finish(a->arc) < 0)
    {
        fprintf(stderr,"Errore archvio finish: %s\n" ,
                archive_error_string(a->arc));
        return false;
    }
#else
    archive_write_finish(a->arc);
#endif

    return true;
}

int add_file(const char *name, archiveData *a)
{
    archive_entry *entry=archive_entry_new();
    struct stat st;
    char buffer[ABLOCK];
    int fd;
    int nread;
    char *tname=NULL;

    if (stat(name, &st) < 0)
        perror("stat() error");

    archive_entry_copy_stat(entry, &st);
    archive_entry_set_size(entry, st.st_size);
    archive_entry_set_mode(entry, st.st_mode);

    tname=strchr(name,'/');
    if (tname!=NULL)
        tname=strchr(tname+1,'/') + 1;

    printf("adding %s...\n",tname);
    archive_entry_set_pathname(entry,tname);

    if (archive_write_header(a->arc, entry) < 0)
    {
        printf("archive_write_header() error");
        printf("%s\n",archive_error_string(a->arc));
    }

    if ((fd=open(name, O_RDONLY)) < 0)
    {
        perror("open() error");
        return -1;
    }

    while ((nread=read(fd,buffer,ABLOCK)) > 0)
        if (archive_write_data(a->arc, buffer, nread) < 0)
            perror("archive_write_data() error");

    if (nread < 0)
    {
        perror("read() error");
        return -1;
    }
    close(fd);
    archive_entry_free(entry);

    return 0;
}

int add_dir(const char *dir, archiveData *a)
{
    struct dirent *pdir;
    struct stat st;
    DIR *d;
    char *path;
    bool added=false;

    path=(char *)malloc(sizeof(char)*PATH_MAX);
    if ((d=opendir(dir)) == 0)
        perror("opendir() error");

    printf("scannind dir %s...\n",dir);

    while((pdir=readdir(d))!=NULL)
    {
        if (strcmp(pdir->d_name,".") == 0)
            continue;
        if (strcmp(pdir->d_name,"..") == 0)
            continue;

        strcpy(path, dir);
        if (path[strlen(path)-1] != '/')
            strcat(path, "/");
        strcat(path, pdir->d_name);

        if (!(lstat(path, &st)>=0))
            continue;

        if (st.st_mode&S_IFREG)
        {
            if (add_file(path, a) == 0)
                added=true;
        }
        else if (st.st_mode&S_IFDIR)
        {
             added+=add_dir(path, a);
        }
    }

    free(path);
    return added;
}

