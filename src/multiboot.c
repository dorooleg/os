#include <memory.h>
#include <printf.h>
#include <multiboot.h>

void print_multiboot_info(void)
{
    multiboot_info_t *mbi = (void *)((uintptr_t)multiboot_info_ptr);

    printf("*** MULTIBOOT ***\n");

    /* Print out the flags. */
    printf ("flags = 0x%x\n", mbi->flags);

    /* Are mem_* valid? */
    if (CHECK_FLAG (mbi->flags, 0)) {
        printf ("mem_lower = %uKB, mem_upper = %uKB\n",
                 mbi->mem_lower,
                 mbi->mem_upper);
    }

    /* Is boot_device valid? */
    if (CHECK_FLAG (mbi->flags, 1)) {
        printf ("boot_device = 0x%x\n", mbi->boot_device);
    }

    /* Is the command line passed? */
    if (CHECK_FLAG (mbi->flags, 2)) {
        printf ("cmdline = %s\n", (char *)(uintptr_t)mbi->cmdline);
    }

    /* Are mods_* valid? */
    if (CHECK_FLAG (mbi->flags, 3)) {
        multiboot_module_t *mod;
        int i;

        printf ("mods_count = %d, mods_addr = 0x%x\n", mbi->mods_count, mbi->mods_addr);
        for (i = 0, mod = (multiboot_module_t *) (uintptr_t)mbi->mods_addr; i < (int)mbi->mods_count; i++, mod++) {
            printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                     mod->mod_start,
                     mod->mod_end,
                     (char *) (uintptr_t)mod->cmdline);
        }
    }

    /* Bits 4 and 5 are mutually exclusive! */
    if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5)) {
        printf ("Both bits 4 and 5 are set.\n");
        return;
    }

    /* Is the symbol table of a.out valid? */
    if (CHECK_FLAG (mbi->flags, 4)) {
        multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);
          
        printf ("multiboot_aout_symbol_table: tabsize = 0x%0x, " "strsize = 0x%x, addr = 0x%x\n",
                 multiboot_aout_sym->tabsize,
                 multiboot_aout_sym->strsize,
                 multiboot_aout_sym->addr);
    }

    /* Is the section header table of ELF valid? */
    if (CHECK_FLAG (mbi->flags, 5)) {
        multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

        printf ("multiboot_elf_sec: num = %u, size = 0x%x,"
                 " addr = 0x%x, shndx = 0x%x\n",
                 multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
                 multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
    }

    /* Are mmap_* valid? */
    printf("Memory mapping:\n");
    if (CHECK_FLAG (mbi->flags, 6)) {
        multiboot_memory_map_t *mmap;
          
        printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
                   mbi->mmap_addr, 
                   mbi->mmap_length);

        for (mmap = (multiboot_memory_map_t *) (uintptr_t)mbi->mmap_addr;
               (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
               mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
            printf(" size = 0x%x, base_addr = 0x%x%x,"
                    " length = 0x%x%x, type = 0x%x\n",
                    mmap->size,
                    mmap->addr >> 32,
                    mmap->addr & 0xffffffff,
                    mmap->len >> 32,
                    mmap->len & 0xffffffff,
                    mmap->type);
        }
    }

    printf("--- MULTIBOOT ---\n");
}
