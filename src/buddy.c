#include <buddy.h> 
#include <virtual_memory.h>
#include <memory.h>
#include <printf.h>

buddy_node* buddy_table;

void setup_buddy(void)
{
    uint32_t max_number_pages = get_physical_memory_size() / PAGE_SIZE;
    uint64_t phys = get_physical_segment(max_number_pages * sizeof(buddy_table));
    buddy_table = (buddy_node*) get_virtual_address(phys);
    buddy_table[0].is_valid = 0;

    int mit = 0; 
    uint32_t prev = 0;
    for (uint32_t i = 0; i < max_number_pages; i++) {
        if (is_available(mit, mit + PAGE_SIZE - 1)) {
            buddy_table[i].is_valid = 1;
            buddy_table[i].mask = 0;
            buddy_table[i].mask_end = 0;
            buddy_table[prev].next = i;
            prev = i;
        }
        mit += PAGE_SIZE;
    }
}

int32_t mark_block(uint64_t page_id, uint64_t block, uint64_t offset)
{
    uint32_t block_offset = 0;
    const uint32_t size = 64 / offset;
    uint64_t bit_end = (1 << (offset - 1));
    for (uint32_t i = 0; i < size; i++) {
        if (!(buddy_table[page_id].mask & block)) {
            buddy_table[page_id].mask |= block;
            buddy_table[page_id].mask_end |= bit_end;
            return block_offset;
        }

        block_offset += 1;
        block <<= offset;
        bit_end <<= offset;
    }
    return -1;
}

uint64_t get_block(uint64_t block, uint64_t offset)
{
    uint32_t max_number_pages = get_physical_memory_size() / PAGE_SIZE;
    uint32_t i = 0;
    while (i < max_number_pages) {
        if (buddy_table[i].is_valid) {
            int32_t block_offset = mark_block(i, block, offset); 
            if (block_offset != -1) {
                return i * PAGE_SIZE + block_offset * BLOCK_SIZE;
            }
        }
        if (i != buddy_table[i].next) {
            i = buddy_table[i].next;
        }
    }
    return 0;
}

uint64_t find_memory(uint8_t level)
{
    switch(level)
    {
       case 1: return get_block(BLOCK1, OFFSET1); 
       case 2: return get_block(BLOCK2, OFFSET2); 
       case 3: return get_block(BLOCK4, OFFSET4); 
       case 4: return get_block(BLOCK8, OFFSET8); 
       case 5: return get_block(BLOCK16, OFFSET16); 
       case 6: return get_block(BLOCK32, OFFSET32); 
       case 7: return get_block(BLOCK64, OFFSET64); 
    }
    return 0;
}

uint64_t alloc_buddy(uint32_t size)
{
    uint64_t exp = 1;
    uint8_t level = 1;
    while (exp < size) {
        exp <<= 2;
        level++;
    }

    if (exp > PAGE_SIZE) {
        return 0;
    }

    uint64_t phys_address = find_memory(level);


    return phys_address ? get_virtual_address(phys_address) : 0;
}

void free_buddy(uint64_t* addr)
{
    if (addr == 0) {
        return;
    }

    uint64_t page_index = get_physical_address((uint64_t)addr) / PAGE_SIZE;  
    uint64_t block_offset = (get_physical_address((uint64_t)addr) - page_index * PAGE_SIZE) / BLOCK_SIZE;
    uint32_t i = 0;
    for (i = block_offset; i < 64; i++) {
        CLR_FLAG(buddy_table[page_index].mask, i); 
        if (CHECK_FLAG(buddy_table[page_index].mask_end, i)) {
            CLR_FLAG(buddy_table[page_index].mask_end, i); 
            break;
        }
    }
}
