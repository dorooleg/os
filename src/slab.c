#include <slab.h>
#include <buddy.h>
#include <memory.h>
#include <printf.h>

static char reserve_slab(slab_metadata* metadata)
{
    metadata->sharing = (void*)alloc_buddy(PAGE_SIZE); 
    if (metadata->sharing == 0) {
        return 0;
    }

    metadata->next = (slab_node*)alloc_buddy(PAGE_SIZE);
    if (metadata->next == 0) {
        return 0;
    }

    slab_node* root = metadata->next; 
    for (int i = 0; i * sizeof(metadata->next) < PAGE_SIZE && (i + 1) * metadata->block_size < PAGE_SIZE; i++) {
        root->next = metadata->next + i * sizeof(metadata->next); 
        root->data = (uint8_t*)metadata->sharing + i * metadata->block_size;
        root->free = 1;
        root = root->next;
    }
    root->next = 0;

    return 1;
}

slab_metadata create_slab_allocator(uint64_t block_size)
{
    slab_metadata metadata; 
    metadata.next = 0;
    metadata.reserve = 0;
    metadata.block_size = block_size;
    reserve_slab(&metadata);
    return metadata;
}

void* alloc_slab(slab_metadata* metadata)
{
    slab_metadata * root = metadata;
    while (root) {
        slab_node* node = metadata->next; 
        while (node) {
            if (node->free) {
                node->free = 0;
                return node->data;
            }
            node = node->next;
        }
        
        if (metadata->reserve == 0) {
            break;
        }

        root = metadata->reserve;
    }

    if (!reserve_slab(root)) {
        return 0;
    }
    
    root->next->free = 0;
    return root->next->data;
}

void free_slab(slab_metadata * metadata, void* addr)
{
    slab_metadata * root = metadata;
    while (root) {
        slab_node* node = metadata->next; 
        while (node) {
            if (node->data == addr) {
                node->free = 1;
                return;
            }
            node = node->next;
        }
        
        if (metadata->reserve == 0) {
            break;
        }

        root = metadata->reserve;
    }
}
