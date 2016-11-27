#ifndef __LIST_H__
#define __LIST_H__

#include <stdint.h>

typedef struct list list_t;
struct list
{
    list_t *next;
    void *value;
};

void list_init();
list_t* list_top(list_t **l);
list_t* list_find(list_t **l, char (*predicate)(void *value));
list_t* list_at(list_t **l, uint32_t pos);
void list_push_back(list_t **l, void *value);
void list_push_front(list_t **l, void *value);
void list_pop_front(list_t **l, void (*destructor)(void * value));
void list_pop_back(list_t **l, void (*destructor)(void * value));
void list_remove_first(list_t **l, char (*predicate)(void *value), void (*destructor)(void * value));
void list_remove_all(list_t **l, char (*predicate)(void *value), void (*destructor)(void * value));
void list_insert_after(list_t **l, char (*predicate)(void *value), void *value);
void list_map(list_t **l, void (*functor)(void *value));
uint32_t list_size(list_t **l);

#endif /*__LIST_H__*/
