#include <fast_slab.h>
#include <pagea.h>
#include <memory.h>

fast_slab_metadata create_fast_slab_allocator(uint64_t block_size)
{
    fast_slab_metadata metadata; 
    metadata.next = 0;
    metadata.block_size = block_size < 8 ? 8 : block_size;
    return metadata;
}

void* get_page(fast_slab_metadata* metadata)
{
    uint8_t* p = pagea_alloc(1);
    
    for (uint64_t i = metadata->block_size; i < PAGE_SIZE; i += metadata->block_size) {
        (*(uint64_t*)(p + i - metadata->block_size)) = (uint64_t)(p + i);
    }

    (*(uint64_t*)(p + PAGE_SIZE / metadata->block_size)) = 0;
    
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
