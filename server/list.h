#ifndef LIST_H
#define LIST_H

#include <string.h>

#ifdef linux
    #include <malloc.h>
#else 
    #include <stdlib.h>
#endif



typedef struct elem elem;
struct elem
{
    void *p;
    elem *next;
    elem *prev;
};


typedef struct list {
    elem *head;
    elem *tail;
} list;

list *init_list ();
void free_list (list *);

void insert_front (list *, void *);
void insert_back (list *, void *);
void insert_after (list *, elem *, void *);
void insert_before (list *, elem *, void *);

void delete_from (list *, elem *);

#endif
