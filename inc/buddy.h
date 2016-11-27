#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <stdint.h>

struct buddy_node_t
{
    uint64_t mask_end;
    uint64_t mask;
    uint64_t next;
    uint64_t prev;
    uint64_t head;
    char     is_valid;
};
typedef struct buddy_node_t buddy_node;

struct buddy_stair_t
{
    buddy_node* next;
};
typedef struct buddy_stair_t buddy_stair;


void setup_buddy(void);
uint64_t alloc_buddy(uint32_t size);
void free_buddy(uint64_t* addr);

uint64_t alloc_buddy_concurrent(uint32_t size);
void free_buddy_concurrent(uint64_t* addr);

#define BLOCK_SIZE 64

#define BLOCK1  0x1LL
#define BLOCK2  0x3LL
#define BLOCK4  0xFLL
#define BLOCK8  0xFFLL
#define BLOCK16 0xFFFFLL
#define BLOCK32 0xFFFFFFFFLL
#define BLOCK64 0xFFFFFFFFFFFFFFFFLL

#define OFFSET1  1
#define OFFSET2  2
#define OFFSET4  4
#define OFFSET8  8
#define OFFSET16 16
#define OFFSET32 32
#define OFFSET64 64

#endif //__BUDDY_H__
