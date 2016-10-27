#ifndef __VIRTUAL_MEMORY_H__
#define __VIRTUAL_MEMORY_H__

#include <stdint.h>
#include <multiboot.h>

#define SET_FLAG(flags,bit)   (flags = (flags) | (1LL << (bit)))
#define CLR_FLAG(flags,bit)   (flags = (flags) ^ (1LL << (bit)))
#define MAX_MEMORY_MAP_SIZE 32
#define PML3_MAX_SIZE 512
#define GB1 0x40000000LL
#define PTE_PRESENT    (1 << 0)
#define PTE_WRITE    (1 << 1)
#define PTE_LARGE    (1 << 7)

struct physical_memory_table
{
    #define MAX_PHYSICAL_MEMORY_SIZE 128
    multiboot_memory_map_t table[MAX_PHYSICAL_MEMORY_SIZE];
    uint32_t size;
};
typedef struct physical_memory_table physical_memory_table_t;

void setup_memory(void);
void print_physical_memory_table(void);
void reserve_physical_memory(uint64_t l, uint64_t r);
uint64_t get_physical_segment(uint64_t size);
void setup_mapping(void);
uint64_t get_physical_segment(uint64_t size);
uint64_t get_physical_memory_size(void);
char is_available(uint64_t l, uint64_t r);

uint64_t get_physical_address(uint64_t virtual_address);
uint64_t get_virtual_address(uint64_t physical_address);

#endif //__VIRTUAL_MEMORY_H__
