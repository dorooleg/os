#include <multiboot.h>
#include <mmap.h>
#include <printf.h>

extern char text_phys_begin[];
extern char bss_phys_end[];

void set_reserved(uint64_t l, uint64_t r)
{
    multiboot_memory_map_t memory_maps[MAX_MEMORY_MAP_SIZE];
    uint32_t memory_maps_length = 0;

    multiboot_info_t *mbi = (void *)((uintptr_t)multiboot_info_ptr);

    if (CHECK_FLAG (mbi->flags, 6)) {
        multiboot_memory_map_t *mmap;
          
        for (mmap = (multiboot_memory_map_t *) (uintptr_t)mbi->mmap_addr;
               (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
               mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
            uint64_t mmapl = mmap->addr;
            uint64_t mmapr = mmap->addr + mmap->len;
            if (r < mmapl || l > mmapr)
            {
                memory_maps[memory_maps_length++] = *mmap;
            }
            else if (mmapl < l && r < mmapr)
            {
                memory_maps[memory_maps_length] = *mmap;
                memory_maps[memory_maps_length++].len = l - mmapl;

                memory_maps[memory_maps_length] = *mmap;
                memory_maps[memory_maps_length].addr += l - mmapl;
                memory_maps[memory_maps_length].len = r - l + 1;
                memory_maps[memory_maps_length++].type = MULTIBOOT_MEMORY_RESERVED;

                memory_maps[memory_maps_length] = *mmap;
                memory_maps[memory_maps_length].addr += r - mmapl + 1;
                memory_maps[memory_maps_length++].len -= r - mmapl + 1;
            } 
            else if (r <= mmapr)
            {
                memory_maps[memory_maps_length] = *mmap;
                memory_maps[memory_maps_length].addr = mmapl;
                memory_maps[memory_maps_length].len = r - mmapl + 1;
                memory_maps[memory_maps_length++].type = MULTIBOOT_MEMORY_RESERVED;

                if ((int)mmapr - (int)r >= 0) {
                    memory_maps[memory_maps_length] = *mmap;
                    memory_maps[memory_maps_length++].addr = r + 1;
                    memory_maps[memory_maps_length++].len = mmapr - r;
                }

                l = r + 1;
            } 
            else if (l <= mmapr)
            {
                if ((int)l - (int)mmapl >= 0) {
                    memory_maps[memory_maps_length] = *mmap;
                    memory_maps[memory_maps_length++].len = l - mmapl;
                }

                memory_maps[memory_maps_length] = *mmap;
                memory_maps[memory_maps_length].addr = l;
                memory_maps[memory_maps_length].len = mmapr - l + 1;
                memory_maps[memory_maps_length++].type = MULTIBOOT_MEMORY_RESERVED;

                l = mmapr + 1;
            }
        }


        mbi->mmap_length = memory_maps_length * sizeof(multiboot_memory_map_t);
        for (int i = 0; i < (int)memory_maps_length; i++) {
            ((multiboot_memory_map_t*)(uintptr_t)mbi->mmap_addr)[i] = memory_maps[i];
        }
    }
}

void setup_memory(void)
{
    uint64_t kernel_begin = (uint64_t) text_phys_begin;
    uint64_t kernel_end = (uint64_t) bss_phys_end;
    set_reserved(kernel_begin, kernel_end);
}


