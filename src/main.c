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
#include <malloc.h>
#include <memory.h>
#include <pagea.h>
#include <fast_slab.h>
#include <test.h>
#include <list.h>
#include <thread.h>
#include <balloc.h>
#include <paging.h>

extern uintptr_t text_phys_begin[];
extern uintptr_t bss_phys_begin[];
extern uintptr_t bss_phys_end[];
extern uintptr_t data_phys_begin[];
extern uintptr_t data_phys_end[];

void test_buddy(void)
{
    struct ext_list_head head;
    unsigned long count = 0;

    ext_list_init(&head);
    while (1) {
        struct page *page = __page_alloc(0);

        if (!page)
            break;
        ++count;
        ext_list_add(&page->ll, &head);
    }

    printf("Allocated %lu pages\n", count);

    while (!ext_list_empty(&head)) {
        struct ext_list_head *node = head.next;
        struct page *page = CONTAINER_OF(node, struct page, ll);

        ext_list_del(&page->ll);
        __page_free(page, 0);
    }
}

void main(void)
{
    qemu_gdb_hang();
    disable_ints();

    init_io();
    idt_install();
    intr_install();
    irq_init();
    pit_init();
    timer_init();

    void *mbi = (void *)((uintptr_t)multiboot_info_ptr);
    balloc_setup(mbi);
    paging_setup();
    page_alloc_setup();

    list_init();
    threads_init();


    disable_ints();
    test_main();
    while (1);
}
