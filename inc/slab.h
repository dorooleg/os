#ifndef __SLAB_H__
#define __SLAB_H__

#include <stdint.h>

typedef struct slab_node_t slab_node;
struct slab_node_t
{
    slab_node*   next;
    void*        data;
    char         free;
};

typedef struct slab_metadata_t slab_metadata;
struct slab_metadata_t
{
    slab_node*      next;
    slab_metadata*  reserve; 
    void*           sharing; 
    uint64_t        block_size;
};

slab_metadata create_slab_allocator(uint64_t block_size);
void* alloc_slab(slab_metadata* metadata);
void free_slab(slab_metadata * metadata, void* addr);

slab_metadata create_slab_allocator_concurrent(uint64_t block_size);
void* alloc_slab_concurrent(slab_metadata* metadata);
void free_slab_concurrent(slab_metadata * metadata, void* addr);

#endif //__SLAB_H__
