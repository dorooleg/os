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
#include <mmap.h>

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
    print_multiboot_info();

    while (1);
}
