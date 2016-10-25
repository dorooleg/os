#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <stdint.h>

struct buddy_node_t
{
    uint64_t mask_end;
    uint64_t mask;
    uint32_t next;
    char     is_valid;
};

typedef struct buddy_node_t buddy_node;

void setup_buddy(void);
uint64_t alloc_buddy(uint32_t size);
void free_buddy(uint64_t* addr);

#define BLOCK_SIZE 128

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
