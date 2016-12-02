#include <condition_variable.h>
#include <lock.h>
#include <ints.h>

extern list_t * running_threads;
static void nothing(void *data) { (void)data; }
extern struct spinlock multithreading_lock;

void cond_wait(struct cond_t * lock_)
{
    locki();
    struct thread_t * current_thread = thread_get_current();
    list_push_back(&lock_->locked_threads, current_thread);
    mutex_thread_yield();
}

void cond_signal(struct cond_t * lock_)
{
    locki();
    if (list_size(&lock_->locked_threads) != 0) {
        struct thread_t* thread = list_top(&lock_->locked_threads)->value;
        list_pop_front(&lock_->locked_threads, nothing);
        list_push_back(&running_threads, thread);
    }
    unlocki();
}

void cond_broadcast(struct cond_t * lock_)
{
    locki();
    while (list_size(&lock_->locked_threads) != 0) {
        struct thread_t* thread = list_top(&lock_->locked_threads)->value;
        list_pop_front(&lock_->locked_threads, nothing);
        list_push_back(&running_threads, thread);
    }
    unlocki();
}
