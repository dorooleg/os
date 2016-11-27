#ifndef __PAGEA_H__
#define __PAGEA_H__

#include <stdint.h>

#define LEFT_SIDE 0
#define RIGHT_SIDE 1

typedef struct pagea_node_t pagea_node;

struct pagea_node_t
{
    pagea_node* next;
    pagea_node* prev;
    uint8_t     side;
    uint8_t     is_empty;
    uint64_t    level;
};

struct pagea_stair_t
{
    pagea_node* next;
};
typedef struct pagea_stair_t pagea_stair;

#define MAX_STAIR 30

void setup_pagea(void);
void fill_stair(uint64_t page_id, uint64_t size);
void print_stair(void);
void* pagea_alloc(uint64_t numer_pages);
void split(uint64_t from, uint64_t to);
void pagea_free(void* p);
void erase_page(uint64_t page_id);
void insert_page(uint64_t page_id, uint64_t level);

void* pagea_alloc_concurrent(uint64_t numer_pages);
void pagea_free_concurrent(void* p);

#endif //__PAGEA_H__
