#include <printf.h>
/*
#include <malloc.h>
#include <slab.h>
#include <buddy.h>
#include <memory.h>
#include <virtual_memory.h>

malloc_node * malloc_root = 0;
slab_metadata slab_meta;

void init_malloc()
{
    slab_meta = create_slab_allocator(sizeof(malloc_node));
}

extern buddy_node* buddy_table;

void* malloc(uint64_t size)
{ 

    uint64_t alloc_number_pages = size / PAGE_SIZE + !(size % PAGE_SIZE == 0) ;
    uint64_t count_pages = 0; 
    uint64_t index_begin_pages = 0; 
    
    uint32_t max_number_pages = get_physical_memory_size() / PAGE_SIZE;
    uint32_t i = 0;
    
    while (i < max_number_pages) {
        if (buddy_table[i].is_valid && !(buddy_table[i].mask & BLOCK64)) {
            index_begin_pages = count_pages ? index_begin_pages : i;
            count_pages++;
            if (count_pages == alloc_number_pages) {
                uint64_t addr = index_begin_pages * PAGE_SIZE;
                while (count_pages--) {
                   buddy_table[index_begin_pages].mask |= BLOCK64; 
                   index_begin_pages = buddy_table[index_begin_pages].next;
                }

                if (malloc_root == 0) {
                    malloc_root = alloc_slab(&slab_meta);
                    if (malloc_root == 0) {
                        return 0;
                    }
                    malloc_root->addr = addr; 
                    malloc_root->len = alloc_number_pages;
                    malloc_root->next = 0;
                    return (void*)get_virtual_address(addr);
                }

                malloc_node * root = malloc_root;
                while (root->next != 0) {
                    root = root->next;
                }

                root->next = alloc_slab(&slab_meta);
                if (root->next == 0) {
                    return 0;
                }
                root->next->addr = addr; 
                root->next->len = alloc_number_pages;
                root->next->next = 0;
                return (void*)get_virtual_address(addr);
            }
        } else {
            index_begin_pages = 0;
            count_pages = 0;
        }

        if (i != buddy_table[i].next) {
            i = buddy_table[i].next;
        }
    }
    return 0;
}

void free(void* addr)
{
    if (addr == 0) {
        return;
    }

    if (malloc_root == 0) {
        return;
    }

    malloc_node * prev = malloc_root;
    malloc_node * root = malloc_root;
    
    while (root->next != 0 && root->addr != (uint64_t)get_physical_address((uint64_t)addr)) {
        prev = root;
        root = root->next;
    }

    if (root->addr == (uint64_t)get_physical_address((uint64_t)addr)) {
        uint64_t index_begin_pages = (uint64_t)get_physical_address((uint64_t)addr) / PAGE_SIZE;
        while (root->len--) {
           buddy_table[index_begin_pages].mask &= 0; 
           index_begin_pages = buddy_table[index_begin_pages].next;
        }
        if (root == malloc_root) {
            free_slab(&slab_meta, malloc_root);
            malloc_root = 0;
        } else {
            prev->next = root->next;
            free_slab(&slab_meta, root);
        }
    }
}
*/
