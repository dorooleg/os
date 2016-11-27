#include <pagea.h>
#include <virtual_memory.h>
#include <memory.h>
#include <printf.h>
#include <spinlock.h>

static struct spinlock memory_lock;

pagea_node*  _pagea_table;
pagea_stair* _pagea_stair;

uint64_t pow(uint64_t x, uint64_t d)
{
    uint64_t n = x;
    for (uint64_t i = 1; i < d; i++) {
       x *= n; 
    }
    return d == 0 ? 1 : x;
}

uint64_t get_2_upper_degree(uint64_t value)
{
    uint64_t degree = 1;
    uint64_t exp = 1;  
    while (exp < value) {
        exp *= 2;
        degree++;
    } 
    return degree;
}

uint64_t get_2_upper_value(uint64_t value)
{
    uint64_t degree = 0;
    uint64_t exp = 1;  
    while (exp < value) {
        exp *= 2;
        degree++;
    } 
    return degree;
}

uint64_t get_2_lower_degree(uint64_t value)
{
    uint64_t degree = 1;
    uint64_t exp = 1;  
    while (exp < value) {
        exp *= 2;
        degree++;
    } 
    return degree != 1 ? degree - 1 : degree;
}

uint64_t get_2_lower_value(uint64_t value)
{
    uint64_t degree = 0;
    uint64_t exp = 1;  
    while (exp < value) {
        exp *= 2;
        degree++;
    } 
    return degree ? exp / 2 : exp;
}

void setup_pagea(void)
{
    uint32_t max_number_pages = get_physical_memory_size() / PAGE_SIZE;
    uint64_t phys = get_physical_segment(max_number_pages * sizeof(_pagea_table));
    _pagea_table = (pagea_node*)get_virtual_address(phys);
    _pagea_stair = (pagea_stair*)get_virtual_address(get_physical_segment(MAX_STAIR * sizeof(_pagea_stair)));

    for (int i = 0; i < MAX_STAIR; i++) {
        _pagea_stair[i].next = 0;
    }

    for (uint64_t i = 0; i < max_number_pages; i++) {
        _pagea_table[i].is_empty = 0; 
    }

    uint64_t mit = 0; 
    uint64_t max_sequence = 0;
    for (uint64_t i = 0; i < max_number_pages; i++) {
        uint64_t cit = i;
        uint64_t cmit = mit;
        while (is_available(cmit, cmit + PAGE_SIZE - 1) && cit < max_number_pages) {
            max_sequence++;
            cmit += PAGE_SIZE;
            cit++;
        }
        fill_stair(i, cit - i);
        mit = cmit + PAGE_SIZE;
        i = cit;
    }
}

void fill_stair(uint64_t page_id, uint64_t size)
{
    if (size == 0) {
        return;
    }

    uint64_t degree = get_2_lower_degree(size);
    _pagea_table[page_id].level = degree; 
    _pagea_table[page_id].side = LEFT_SIDE; 
    _pagea_table[page_id].prev = 0; 
    _pagea_table[page_id].is_empty = 1; 
    _pagea_table[page_id].next = _pagea_stair[degree].next; 
    _pagea_stair[degree].next = _pagea_table + page_id;

    uint64_t exp = get_2_lower_value(size);
    fill_stair(page_id + exp, size - exp);
}

void* pagea_alloc(uint64_t number_pages)
{
    uint64_t level = get_2_upper_degree(number_pages);
    for (uint64_t l = level; l < MAX_STAIR; l++) {
        if (_pagea_stair[l].next) {
            split(l, level);
            pagea_node* node = _pagea_stair[level].next;
            node->is_empty = 0;
            node->next = 0;
            _pagea_stair[level].next = node->next;
            uint64_t page_id = node - _pagea_table;
            return (void*)get_virtual_address(page_id * PAGE_SIZE);
        }
    }
    return 0;
}

void merge(uint64_t page_index)
{
    uint64_t level = _pagea_table[page_index].level;
    switch (_pagea_table[page_index].side)
    {
        case LEFT_SIDE: {
            uint32_t max_number_pages = get_physical_memory_size() / PAGE_SIZE;
            uint64_t right_page_index = page_index + pow(1, level) + 1;
            if (right_page_index >= max_number_pages) {
                break;
            }
            if (_pagea_table[right_page_index].level == level && 
                _pagea_table[right_page_index].is_empty && 
                _pagea_table[right_page_index].side == RIGHT_SIDE) {

               erase_page(page_index); 
               erase_page(right_page_index); 
               _pagea_table[page_index].side = LEFT_SIDE;
               insert_page(page_index, level + 1);    
            }
        }
        break;
        case RIGHT_SIDE: {
            uint64_t left_page_index = page_index - pow(1, level) - 1;
            if (_pagea_table[left_page_index].level == level && 
                _pagea_table[left_page_index].is_empty && 
                _pagea_table[left_page_index].side == LEFT_SIDE) {

                erase_page(page_index); 
                erase_page(left_page_index); 
                _pagea_table[left_page_index].side = LEFT_SIDE;
                insert_page(left_page_index, level + 1);    
            }
        }
        break;
    }
}

void erase_page(uint64_t page_id)
{
    if (_pagea_table[page_id].is_empty) {
        return;
    }
    _pagea_table[page_id].is_empty = 0;
    uint64_t level = _pagea_table[page_id].level;
    if (_pagea_table[page_id].prev == 0) {
        _pagea_stair[level].next = _pagea_table[page_id].next; 
        if (_pagea_table[level].next) {
            _pagea_table[level].next->prev = 0;
        }
    }
}

void insert_page(uint64_t page_id, uint64_t level)
{
    _pagea_table[page_id].is_empty = 1;
    _pagea_table[page_id].level = level;
    _pagea_table[level].prev = 0;
    _pagea_table[level].next = _pagea_stair[level].next;
    if (_pagea_stair[level].next) {
        _pagea_stair[level].next->prev = _pagea_table + page_id;
    }
    _pagea_stair[level].next = _pagea_table + page_id; 
}

void pagea_free(void* p)
{
    if (p == 0) {
        return;
    }

    uint64_t page_index = get_physical_address((uint64_t)p) / PAGE_SIZE;  
    uint64_t level = _pagea_table[page_index].level;

    _pagea_table[page_index].is_empty = 1;
    pagea_node* node = _pagea_table + page_index;
    node->next = _pagea_stair[level].next;
    if (_pagea_stair[level].next) {
        _pagea_stair[level].next->prev = node;
    }
    
    _pagea_stair[level].next = node;

    merge(page_index);
}

void split(uint64_t from, uint64_t to)
{
    if (from == to) {
        return;
    }

    pagea_node* node = _pagea_stair[from].next;
    _pagea_stair[from].next = node->next; 

    uint64_t degree = pow(1, from - 1);
    node->level = from - 1;
    node->side = LEFT_SIDE;
    node->next = node + degree + 1; 
    node->is_empty = 1;
    node->prev = 0;

    (node + degree + 1)->level = from - 1;
    (node + degree + 1)->side = RIGHT_SIDE;
    (node + degree + 1)->next = _pagea_stair[from - 1].next;
    (node + degree + 1)->is_empty = 1;

    _pagea_stair[from - 1].next = node;

    split(from - 1, to);
}

void print_stair(void)
{
    for (int i = 0; i < MAX_STAIR; i++) {
        printf("%i: ", i);
        pagea_node* node = _pagea_stair[i].next;
        while (node) {
            printf("%x ", get_virtual_address(PAGE_SIZE * (node - _pagea_table)));
            node = node->next;
        } 
        printf("\n");
    }
}

void* pagea_alloc_concurrent(uint64_t numer_pages)
{
    lock(&memory_lock);
    void *p = pagea_alloc(numer_pages);
    unlock(&memory_lock);
    return p;
}

void pagea_free_concurrent(void* p)
{
    lock(&memory_lock);
    pagea_free(p);
    unlock(&memory_lock);
}
