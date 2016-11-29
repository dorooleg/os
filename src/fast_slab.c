#include <fast_slab.h>
#include <pagea.h>
#include <memory.h>
#include <spinlock.h>


fast_slab_metadata create_fast_slab_allocator(uint64_t block_size)
{
    fast_slab_metadata metadata; 
    metadata.next = 0;
    metadata.memory_lock.locked = 0;
    metadata.block_size = block_size < 8 ? 8 : block_size;
    return metadata;
}

void* get_page(fast_slab_metadata* metadata)
{
    uint8_t* p = pagea_alloc(1);
    for (uint64_t i = 0; i < PAGE_SIZE; i++) {
        p[i] = 0;
    }
    
    for (uint64_t i = metadata->block_size; i < PAGE_SIZE; i += metadata->block_size) {
        (*(uint64_t*)(p + i - metadata->block_size)) = (uint64_t)(p + i);
    }
    
    return p;
}

void* alloc_fast_slab(fast_slab_metadata* metadata)
{
    if (!metadata) {
        return 0;
    }
    
    if (metadata->next) {
        void* addr = metadata->next; 
        metadata->next = (void*)(*((uint64_t*)addr));
        return addr;
    }

    metadata->next = get_page(metadata);
    
    if (metadata->next) {
        void* addr = metadata->next; 
        metadata->next = (void*)(*((uint64_t*)addr));
        return addr;
    }

    return 0;
}

void free_fast_slab(fast_slab_metadata * metadata, void* addr)
{
    if (!metadata) {
        return;
    }
    *((uint64_t*)addr) = (uint64_t)metadata->next;
    metadata->next = addr;
}

void* alloc_fast_slab_concurrent(fast_slab_metadata* metadata)
{
    lock(&metadata->memory_lock);
    void* ptr = alloc_fast_slab(metadata);
    unlock(&metadata->memory_lock);
    return ptr;
}

void free_fast_slab_concurrent(fast_slab_metadata * metadata, void* addr)
{
    lock(&metadata->memory_lock);
    free_fast_slab(metadata, addr);
    unlock(&metadata->memory_lock);
}

fast_slab_metadata create_fast_slab_allocator_concurrent(uint64_t block_size)
{
    fast_slab_metadata meta = create_fast_slab_allocator(block_size);
    return meta;
}
