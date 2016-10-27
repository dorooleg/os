#include <buddy.h> 
#include <virtual_memory.h>
#include <memory.h>
#include <printf.h>

buddy_node* buddy_table;
buddy_stair* _buddy_stair;

void setup_buddy(void)
{
    uint32_t max_number_pages = get_physical_memory_size() / PAGE_SIZE;
    uint64_t phys = get_physical_segment(max_number_pages * sizeof(buddy_table));
    buddy_table = (buddy_node*) get_virtual_address(phys);
    _buddy_stair = (buddy_stair*) get_virtual_address(get_physical_segment(8 * sizeof(buddy_stair)));
    buddy_table[0].is_valid = 0;
    for (int i = 0; i < 8; i++) {
        _buddy_stair[i].next = 0;
    }

    uint64_t mit = 0; 
    uint64_t prev = 0;
    for (uint32_t i = 0; i < max_number_pages; i++) {
        buddy_table[i].mask = 0;
        buddy_table[i].is_valid = 0;
        buddy_table[i].next = 0;
        buddy_table[i].prev = 0;
        buddy_table[i].head = 0;
        if (is_available(mit, mit + PAGE_SIZE - 1)) {
            if (prev == 0) {
                _buddy_stair[7].next = buddy_table + i;
            }

            buddy_table[i].is_valid = 1;
            buddy_table[i].mask = 0;
            buddy_table[i].mask_end = 0;
            buddy_table[i].head = (uint64_t)_buddy_stair;
            buddy_table[prev].next = i;
            buddy_table[prev].prev = prev;
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

char check_internal_level(uint64_t page_id, uint64_t block, uint64_t offset)
{
    const uint32_t size = 64 / offset;
    for (uint32_t i = 0; i < size; i++) {
        if (!(buddy_table[page_id].mask & block)) {
            return 1;
        }

        block <<= offset;
    }
    return 0;
}

char check_level(uint64_t l, uint64_t page_id)
{
    switch(l)
    {
       case 1: return check_internal_level(page_id, BLOCK1, OFFSET1); 
       case 2: return check_internal_level(page_id, BLOCK2, OFFSET2); 
       case 3: return check_internal_level(page_id, BLOCK4, OFFSET4); 
       case 4: return check_internal_level(page_id, BLOCK8, OFFSET8); 
       case 5: return check_internal_level(page_id, BLOCK16, OFFSET16); 
       case 6: return check_internal_level(page_id, BLOCK32, OFFSET32); 
       case 7: return check_internal_level(page_id, BLOCK64, OFFSET64); 
    }
    return 0;
}

uint8_t up_level(uint8_t l, uint64_t page_id)
{
    uint8_t max_level = 0;
    for (; l < 8; l++) {
        if (check_level(l, page_id)) {
            max_level = l;
        }
    }
    return max_level;
}

uint8_t down_level(uint8_t l, uint64_t page_id)
{
    uint8_t max_level = 0;
    for (; l > 0; l--) {
        if (check_level(l, page_id)) {
            max_level = l;
        }
    }
    return max_level;
}

uint64_t get_block(uint8_t l, uint64_t block, uint64_t offset)
{
    for (; l < 8; l++) {
        if(_buddy_stair[l].next) {
            uint32_t i = _buddy_stair[l].next - buddy_table;
            int32_t block_offset = mark_block(i, block, offset); 
            uint8_t new_level = down_level(l, i);

            if (new_level != l) {
                buddy_node* head = _buddy_stair[l].next;
                _buddy_stair[l].next  = buddy_table + head->next;
                _buddy_stair[l].next->prev = 0;
                head->next = _buddy_stair[new_level].next - buddy_table;
                _buddy_stair[new_level].next->prev = head - buddy_table;
                head->prev = 0;
                head->head = (uint64_t)(_buddy_stair + new_level);
                _buddy_stair[new_level].next = head;
            }

            if (block_offset != -1) {
                return i * PAGE_SIZE + block_offset * BLOCK_SIZE;
            }
        }
    }

    return 0;
}

uint64_t find_memory(uint8_t level)
{
    switch(level)
    {
       case 1: return get_block(level, BLOCK1, OFFSET1); 
       case 2: return get_block(level, BLOCK2, OFFSET2); 
       case 3: return get_block(level, BLOCK4, OFFSET4); 
       case 4: return get_block(level, BLOCK8, OFFSET8); 
       case 5: return get_block(level, BLOCK16, OFFSET16); 
       case 6: return get_block(level, BLOCK32, OFFSET32); 
       case 7: return get_block(level, BLOCK64, OFFSET64); 
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

    uint8_t l = (buddy_stair*)buddy_table[page_index].head - _buddy_stair;
    uint8_t new_level = up_level(l, page_index);

    if (new_level != l) {
        buddy_node* head = _buddy_stair[l].next;
        _buddy_stair[l].next  = buddy_table + head->next;
        _buddy_stair[l].next->prev = 0;
        head->next = _buddy_stair[new_level].next - buddy_table;
        _buddy_stair[new_level].next->prev = head - buddy_table;
        head->prev = 0;
        head->head = (uint64_t)(_buddy_stair + new_level);
        _buddy_stair[new_level].next = head;
    }
}
