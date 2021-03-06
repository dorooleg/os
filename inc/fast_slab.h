#ifndef __FAST_SLAB_H__
#define __FAST_SLAB_H__

#include <stdint.h>
#include <spinlock.h>

typedef struct fast_slab_metadata_t fast_slab_metadata;
struct fast_slab_metadata_t
{
    struct spinlock memory_lock;
    void*           next; 
    uint64_t        block_size;
};

void* alloc_fast_slab(fast_slab_metadata* metadata);
void free_fast_slab(fast_slab_metadata * metadata, void* addr);
fast_slab_metadata create_fast_slab_allocator(uint64_t block_size);

void* alloc_fast_slab_concurrent(fast_slab_metadata* metadata);
void free_fast_slab_concurrent(fast_slab_metadata * metadata, void* addr);
fast_slab_metadata create_fast_slab_allocator_concurrent(uint64_t block_size);

#endif //__FAST_SLAB_H__
