#include <spinlock.h>
#include <printf.h>
#include <ints.h>

void lock(struct spinlock * lock)
{
    do
    {
        while (atomic_load_explicit(&lock->locked, memory_order_acquire) == LOCKED);
    } while (atomic_exchange_explicit(&lock->locked, LOCKED, memory_order_acquire) == LOCKED);
    disable_ints();
}

char try_lock(struct spinlock * lock)
{
    return atomic_exchange_explicit(&lock->locked, LOCKED, memory_order_acquire) == UNLOCKED;
}

void unlock(struct spinlock * lock)
{
    atomic_store_explicit(&lock->locked, UNLOCKED, memory_order_release);
    enable_ints();
}
