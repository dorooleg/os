#ifndef __MALLOC_H_
#define __MALLOC_H_

#include <stdint.h>


typedef struct malloc_node_t malloc_node;
struct malloc_node_t
{
    malloc_node*   next;
    uint64_t       addr; 
    uint64_t       len;
};

void init_malloc();
void* malloc(uint64_t size);
void free(void* addr);


#endif //__MALLOC_H_
