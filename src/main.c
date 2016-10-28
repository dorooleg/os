//#define DEBUG
static void qemu_gdb_hang(void)
{
#ifdef DEBUG
    static volatile int wait = 1;

    while (wait);
#endif
}


#include <desc.h>
#include <ints.h>
#include <iolib.h>
#include <interruptlib.h>
#include <pitlib.h>
#include <irqlib.h>
#include <ioport.h>
#include <printf.h>
#include <backtrace.h>
#include <ints.h>
#include <multiboot.h>
#include <virtual_memory.h>
#include <buddy.h>
//#include <slab.h>
#include <malloc.h>
#include <memory.h>
#include <pagea.h>
#include <fast_slab.h>

extern uintptr_t text_phys_begin[];
extern uintptr_t bss_phys_begin[];
extern uintptr_t bss_phys_end[];
extern uintptr_t data_phys_begin[];
extern uintptr_t data_phys_end[];

void foo2();
void foo1()
{
    foo2();
}

void foo2()
{
    backtrace();
}

void test_buddy(void)
{
    unsigned long count = 0;

    while (1) {
        void *ptr;

        if (!(ptr = pagea_alloc(1)))
            break;
    //    printf("returned %x\n", ptr);
        ++count;
    }
    printf("allocated %ld\n", count);
}

void test_slab(void)
{
    unsigned long count = 0;

    fast_slab_metadata slab = create_fast_slab_allocator(16);
    while (1) {
        void *ptr;

        if (!(ptr = alloc_fast_slab(&slab)))
            break;
        ++count;
        if (count > 1000) {
            break;
        }
    }
    printf("allocated %ld\n", count);
}

void test_buddy2(void)
{
    for (int i = 0; i < 1024; i++) {
        void *ptr;

        if (!(ptr = pagea_alloc(1)))
            break;
        pagea_free(ptr);
    }
}

extern buddy_node* buddy_table;
void main(void)
{
    qemu_gdb_hang();
    init_io();
    idt_install();
    intr_install();
    irq_init();
    pit_init();
    timer_init();

    interrupt(1);

    disable_ints();

    foo1();
    //printf("Hello %corld%c%s %o %lli", 'w', '!', "One", 15, 213278569235LL);
    printf("text_phys_begin %p\n", text_phys_begin);
    printf("bss_phys_begin %p\n", bss_phys_begin);
    printf("bss_phys_end %p\n", bss_phys_end);
    printf("data_phys_begin %p\n", data_phys_begin);
    printf("data_phys_end %p\n", data_phys_end);

    print_multiboot_info();
    setup_memory();
    print_physical_memory_table();

/*
    setup_buddy();
    void *p = (void*)alloc_buddy(67);
    printf("%x\n", p);
    uint64_t page_index = get_physical_address((uint64_t)p) / PAGE_SIZE;  
    printf("[%x]", buddy_table[page_index].mask);   
  
    printf("%x\n", alloc_buddy(1));
*/  

    setup_mapping();    
    print_physical_memory_table();
    setup_pagea();
    
    fast_slab_metadata slab = create_fast_slab_allocator(16);
    void *ptr = alloc_fast_slab(&slab);
    printf("%x\n", ptr);
    void *ptr1 = alloc_fast_slab(&slab);
    printf("%x\n", ptr1);
    void *ptr2 = alloc_fast_slab(&slab);
    printf("%x\n", ptr2);
    free_fast_slab(&slab, ptr2);
    void *ptr3 = alloc_fast_slab(&slab);
    printf("%x\n", ptr3);

    test_slab();
/*
    print_stair();
*/
//    test_buddy();

    //test_buddy2();
    //print_stair();

#if 0
    void * p = pagea_alloc(2048); 
    print_stair();
    pagea_free(p);
    //printf("%x\n", p);
    print_stair();
    //setup_buddy();
    //print_physical_memory_table();
/*
    {
        uint64_t* arr = (uint64_t*)alloc_buddy(1);
        printf("%x\n", arr);
        arr = (uint64_t*)alloc_buddy(4);
        printf("%x\n", arr);
        arr = (uint64_t*)alloc_buddy(4);
        printf("%x\n", arr);
        arr = (uint64_t*)alloc_buddy(PAGE_SIZE);
        printf("%x\n", arr);
        arr = (uint64_t*)alloc_buddy(PAGE_SIZE);
        printf("%x\n", arr);
    }
    uint64_t * arr = (uint64_t*)alloc_buddy(32 * sizeof(uint64_t));
    printf("%x\n", arr);
    free_buddy(arr);
    arr = (uint64_t*)alloc_buddy(32 * sizeof(uint64_t));
    printf("%x\n", arr);
    free_buddy(arr);
    arr = (uint64_t*)alloc_buddy(32 * sizeof(uint64_t));
    printf("%x\n", arr);
    arr = (uint64_t*)alloc_buddy(13 * sizeof(uint64_t));
    printf("%x\n", arr);

    arr[0] = 0;
    arr[1] = 1;
    for (uint32_t i = 2; i < 32; i++) {
        arr[i] = arr[i - 1] + arr[i - 2];
    }
    printf("%i", arr[10]);

    printf("now\n");
    slab_metadata slab64 = create_slab_allocator(64);
    printf("now\n");
    uint8_t * data = alloc_slab(&slab64);
    data[0] = 0;
    data[1] = 1;
    for (uint32_t i = 2; i < 8; i++) {
        data[i] = data[i - 1] + data[i - 2];
    }
    printf("fib = %i\n", data[7]);
    printf("before_free = %x\n", data);
    free_slab(&slab64, data);
    data = alloc_slab(&slab64);

    printf("after_free = %x\n", data);
    init_malloc();

    uint64_t * big_fib = (uint64_t*)malloc(4096 * sizeof(uint64_t));
    printf("big_fib = %x\n", big_fib);
        
    big_fib[0] = 1;
    for (int i = 1; i < 4096; i++) {
        
        big_fib[i] = 1 ^ big_fib[i-1];
    }
    printf("big_fib = %x\n", big_fib[4095]);
    free(big_fib);
    big_fib = (uint64_t*)malloc(4096 * sizeof(uint64_t));
    printf("big_fib = %x\n", big_fib);
*/
#endif
    while (1);
}
