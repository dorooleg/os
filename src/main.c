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
#include <malloc.h>
#include <memory.h>
#include <pagea.h>
#include <fast_slab.h>
#include <test.h>
#include <list.h>
#include <thread.h>

extern uintptr_t text_phys_begin[];
extern uintptr_t bss_phys_begin[];
extern uintptr_t bss_phys_end[];
extern uintptr_t data_phys_begin[];
extern uintptr_t data_phys_end[];

void main(void)
{
    disable_ints();
    qemu_gdb_hang();
    init_io();
    idt_install();
    intr_install();
    irq_init();
    pit_init();
    timer_init();

    interrupt(1);


    printf("text_phys_begin %p\n", text_phys_begin);
    printf("bss_phys_begin %p\n", bss_phys_begin);
    printf("bss_phys_end %p\n", bss_phys_end);
    printf("data_phys_begin %p\n", data_phys_begin);
    printf("data_phys_end %p\n", data_phys_end);

    print_multiboot_info();
    setup_memory();
    print_physical_memory_table();

    setup_mapping();    
    print_physical_memory_table();
    setup_pagea();
    list_init();
    threads_init();
    disable_ints();
    enable_ints();
    test_main();
    while (1);
}
