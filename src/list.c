#include <list.h>
#include <fast_slab.h>
#include <printf.h>

#define NULL 0

fast_slab_metadata list_allocator;  

void list_init()
{
    list_allocator = create_fast_slab_allocator_concurrent(sizeof(list_t));
}

list_t* list_top(list_t **l)
{
    return *l;
}

void list_push_front(list_t **l, void *value)
{
    list_t *n = *l;
    *l = alloc_fast_slab_concurrent(&list_allocator);
    (*l)->next = n;
    (*l)->value = value;
}

list_t* list_find(list_t **l, char (*predicate)(void *value))
{
    list_t *p = *l;
    while (p != NULL) {
        if (predicate(p->value))
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

list_t* list_at(list_t **l, uint32_t pos)
{
    list_t *p = *l;
    uint32_t size = 0; 
    while (p != NULL) {
        if (size == pos) 
            return p;
        size++;
        p = p->next;
    }
    return NULL;
}
void list_push_back(list_t **l, void *value)
{
    list_t **p = l;
    while (*p != NULL) {
        p = &((*p)->next);
    }
    list_push_front(p, value);
}

void list_pop_front(list_t **l, void (*destructor)(void * value))
{
    if (*l != NULL) {
        destructor((*l)->value); 
        list_t *n = (*l)->next;
        free_fast_slab_concurrent(&list_allocator, *l); 
        *l = n; 
    }
}

void list_pop_back(list_t **l, void (*destructor)(void * value))
{
    list_t **f = l;
    list_t **s = l;
    while (*s != NULL) {
        f = s;
        s = &((*s)->next);
    }
    list_pop_front(f, destructor);
}

uint32_t list_size(list_t **l)
{
    list_t *p = *l;
    uint32_t size = 0; 
    while (p != NULL) {
        size++;
        p = p->next;
    }
    return size;
}

void list_remove_first(list_t **l, char (*predicate)(void *value), void (*destructor)(void * value))
{
    list_t *f = *l;
    list_t *s = *l;

    while (s != NULL && !predicate(s->value)) {
        f = s;
        s = s->next;
    }

    if (s != NULL) {
        if (f != s) {
            f->next = s->next;
            destructor(s->value); 
            free_fast_slab_concurrent(&list_allocator, s); 
        } else {
            list_pop_front(l, destructor);
        } 
    }
}

void list_remove_all(list_t **l, char (*predicate)(void *value), void (*destructor)(void * value))
{
    list_t *f = *l;
    list_t *s = *l;
    while (s != NULL) {
        if (predicate(s->value)) {        
            if (f != s) {
                f->next = s->next;
                destructor(s->value); 
                free_fast_slab_concurrent(&list_allocator, s); 
                s = f->next;
            } else {
                list_pop_front(l, destructor);
                f = *l;
                s = *l;
            } 
        } else {
            f = s;
            s = s->next;
        }
    }
}

void list_insert_after(list_t **l, char (*predicate)(void *value), void *value)
{
    list_t *p = *l;
    while (p != NULL) {
        if (predicate(p->value)) {
            list_t *n = p->next;
            list_t * node = alloc_fast_slab_concurrent(&list_allocator);
            node->next = n;
            node->value = value;
            p->next = node;
            return; 
        }
        p = p->next;
    }
}

void list_map(list_t **l, void (*functor)(void *value))
{
    list_t *p = *l;
    uint32_t size = 0; 
    while (p != NULL) {
        functor(p->value);
        size++;
        p = p->next;
    }
}
