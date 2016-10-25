#include <multiboot.h>
#include <virtual_memory.h>
#include <printf.h>
#include <memory.h>
#include <multiboot.h>

physical_memory_table_t physical_memory_table;

uint64_t* pml3_table[PML3_MAX_SIZE];

uint64_t get_physical_memory_size(void)
{
    static uint64_t memory_size = 0;
    if (memory_size == 0) {
        multiboot_memory_map_t * mmap = physical_memory_table.table;
        for (uint32_t i = 0; i < physical_memory_table.size; i++) {
            memory_size += mmap[i].len;
        }
    }
    return memory_size;
}

uint64_t* get_pml4(void)
{
    extern uint64_t* bootstrap_pml4;
    return bootstrap_pml4;
}

void map_1gb_page(uint64_t physical_address, uint64_t virtual_address)
{
    uint64_t page_index = get_physical_address(virtual_address) / GB1;
    uint64_t pml3_index = page_index / PML3_MAX_SIZE + PML3_MAX_SIZE / 2;
    page_index %= PML3_MAX_SIZE; 
    pml3_table[pml3_index][page_index] = physical_address | PTE_WRITE |  PTE_PRESENT | PTE_LARGE;  
    get_pml4()[pml3_index] = get_physical_address((uint64_t)pml3_table[pml3_index]) | PTE_PRESENT | PTE_WRITE;
}

void setup_mapping(void)
{ 
    uint64_t max_number_page = get_physical_memory_size() / GB1 + !(get_physical_memory_size() % GB1 == 0); 
    uint64_t pml3_internal_size = max_number_page / PML3_MAX_SIZE + !(max_number_page % PML3_MAX_SIZE == 0);

    for (uint32_t i = 0; i < pml3_internal_size; i++) {
        pml3_table[i + PML3_MAX_SIZE / 2] = (uint64_t*) get_virtual_address(get_physical_segment(PAGE_SIZE));
    }

    for (uint32_t i = 0; i < max_number_page; i++) {
        map_1gb_page(i * GB1, i * GB1 + VIRTUAL_OFFSET); 
    }
}

uint64_t get_physical_segment(uint64_t size)
{
    multiboot_memory_map_t * mmap = physical_memory_table.table;
    for (uint32_t i = 0; i < physical_memory_table.size; i++) {
        if (mmap[i].len >= size && mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
            uint64_t addr = mmap[i].addr;
            reserve_physical_memory(mmap[i].addr, mmap[i].addr + size);
            return addr;
        } 
    }
    return 0;
}

char is_available(uint64_t l, uint64_t r)
{
    multiboot_memory_map_t * mmap = physical_memory_table.table;
    for (uint32_t i = 0; i < physical_memory_table.size; i++) {
        if (mmap[i].addr <= l && mmap[i].addr <= r && 
            l < mmap[i].addr + mmap[i].len && r < mmap[i].addr + mmap[i].len &&
            mmap[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
            return 1;
        } 
    }
    return 0;
}

uint64_t get_physical_address(uint64_t virtual_address)
{
    return virtual_address - VIRTUAL_OFFSET;
}

uint64_t get_virtual_address(uint64_t physical_address)
{
    return physical_address + VIRTUAL_OFFSET;
}

static void load_physical_memory_table(void)
{
    physical_memory_table.size = 0;
    multiboot_info_t *mbi = (void *)((uintptr_t)multiboot_info_ptr);

    if (CHECK_FLAG (mbi->flags, 6)) {
        multiboot_memory_map_t *mmap;
          
        for (mmap = (multiboot_memory_map_t *) (uintptr_t)mbi->mmap_addr;
             (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
             mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
            physical_memory_table.table[physical_memory_table.size++] = *mmap;
        }
    }
}

void remove_reserved(void)
{
    multiboot_memory_map_t * mmap = physical_memory_table.table;
    uint32_t l = 0;
    
    for (uint32_t r = 0; r < physical_memory_table.size; r++) {
        if (mmap[r].type == MULTIBOOT_MEMORY_AVAILABLE) {
            mmap[l++] = mmap[r];
        }
    }
    physical_memory_table.size = l;
}

void reserve_physical_memory(uint64_t l, uint64_t r)
{
    if (physical_memory_table.size == 0) {
        return;
    }

    if (l > r) {
        return;
    }

    multiboot_memory_map_t * mmap = physical_memory_table.table;

    if (l < mmap[0].addr) {
        return;
    }

    if (r >= mmap[physical_memory_table.size - 1].addr + mmap[physical_memory_table.size - 1].len) {
        return;
    }

    
    uint32_t li = 0;
    for (uint32_t i = 0; i < physical_memory_table.size; i++) {
        if (mmap[i].addr <= l && l < mmap[i].addr + mmap[i].len) {
            li = i;
            break;
        } 
    }

    if (mmap[li].addr != l) {
        multiboot_memory_map_t entry = mmap[li];
        mmap[li].len = l - mmap[li].addr;
        entry.addr = l;
        entry.len -= mmap[li].len;
        li++;
        for (uint32_t i = li; i <= physical_memory_table.size; i++) {
            multiboot_memory_map_t tmp = mmap[i];
            mmap[i] = entry;
            entry = tmp;
        }
        physical_memory_table.size++;
    }

    uint32_t ri = 0;
    for (uint32_t i = 0; i < physical_memory_table.size; i++) {
        if (mmap[i].addr <= r && r < mmap[i].addr + mmap[i].len) {
            ri = i;
            break;
        } 
    }

    if (mmap[ri].addr + mmap[ri].len != r + 1) {
        multiboot_memory_map_t entry = mmap[ri];
        mmap[ri].len = r - mmap[ri].addr + 1;
        entry.addr = r + 1;
        entry.len -= mmap[ri].len;
        for (uint32_t i = ri + 1; i <= physical_memory_table.size; i++) {
            multiboot_memory_map_t tmp = mmap[i];
            mmap[i] = entry;
            entry = tmp;
        }
        physical_memory_table.size++;
    }

    for (; li <= ri; li++) {
        mmap[li].type = MULTIBOOT_MEMORY_RESERVED;
    }
}

void setup_memory(void)
{
    load_physical_memory_table();
    extern char text_phys_begin[];
    extern char bss_phys_end[];
    uint64_t kernel_begin = (uint64_t) text_phys_begin;
    uint64_t kernel_end = (uint64_t) bss_phys_end;
    physical_memory_table.table[0].type = MULTIBOOT_MEMORY_RESERVED;
//    remove_reserved();
    reserve_physical_memory(kernel_begin, kernel_end);
}

void print_physical_memory_table(void)
{
    printf("%i\n", physical_memory_table.size);
    multiboot_memory_map_t * mmap = physical_memory_table.table;
    for (uint32_t i = 0; i < physical_memory_table.size; i++) {
        printf(" size = 0x%x, base_addr = 0x%x%x,"
                " length = 0x%x%x, type = 0x%x\n",
                mmap[i].size,
                mmap[i].addr >> 32,
                mmap[i].addr & 0xffffffff,
                mmap[i].len >> 32,
                mmap[i].len & 0xffffffff,
                mmap[i].type);
    }
}
