#include "list.h"

list *init_list () 
{
    list* l = (list *) malloc (sizeof (list));
    l->head = l->tail = NULL;
    return l;
}

void free_list (list* l) 
{
    elem* cur = l->head;
    elem* next = NULL;

    while (cur) 
    {
        next = cur->next;
        free (cur);
        cur = next;
    }
    free (l);
}

void insert_after (list* l, elem* prev, void *p) 
{
    elem* next = prev ? prev->next : l->head;

    elem* newnode = (elem *) malloc (sizeof (elem));
    newnode->p = p;

    newnode->prev = prev;
    newnode->next = next;

    prev ? prev->next = newnode : 0 ;
    next ? next->prev = newnode : 0 ;

    l->head == NULL ? l->head = l->tail = newnode :
        prev == NULL ? l->head = newnode : 0 ;
}

void insert_before (list* l, elem* next, void *p) 
{
    elem* prev = next ? next->prev : l->tail;

    elem* newnode = (elem *) malloc (sizeof (elem));
    newnode->p = p;

    newnode->prev = prev;
    newnode->next = next;

    next ? next->prev = newnode : 0 ;
    prev ? prev->next = newnode : 0 ;

    l->tail == NULL ? l->head = l->tail = newnode :
        next == NULL ? l->tail = newnode : 0 ;
}

void insert_front (list* l, void *p) 
{
    insert_after (l, NULL, p);
}

void insert_back (list* l, void *p) 
{
    insert_before (l, NULL, p);
}

void delete_from (list *l, elem *node) 
{
    elem* prev = node->prev;
    elem* next = node->next;

    prev ? prev->next = next : 0 ;
    next ? next->prev = prev : 0 ;

    node == l->head ? l->head = next : 0 ;
    node == l->tail ? l->tail = prev : 0 ;

    if(node)
    {
        if(node->p)
            free(node->p);
        free (node);
    }
}
