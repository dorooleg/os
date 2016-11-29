#include <pagea.h>
#include <memory.h>
#include <spinlock.h>

static struct spinlock memory_lock;

void *pagea_alloc(uint64_t order)
{
    struct page *page = __page_alloc(order);

    if (!page) {
        unlock(&memory_lock);
        return 0;
    }

    page->u.order = order;


    return  va(page_addr(page));
}

void pagea_free(void *ptr)
{
    if (!ptr)
        return;

    struct page *page = addr_page(pa(ptr) & ~(uintptr_t)PAGE_MASK);

    const int order = page->u.order;
    const uintptr_t mask = (1ull << (order + PAGE_SHIFT)) - 1;

    page->u.order = 0;
    page_free((~mask & pa(ptr)), order);
}

void* pagea_alloc_concurrent(uint64_t order)
{
    lock(&memory_lock);
    void *p = pagea_alloc(order);
    unlock(&memory_lock);
    return p;
}

void pagea_free_concurrent(void* ptr)
{

    lock(&memory_lock);
    pagea_free(ptr);
    unlock(&memory_lock);
}
