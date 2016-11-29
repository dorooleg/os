#include <mutex.h>
#include <spinlock.h>
#include <ints.h>

extern list_t * running_threads;
static void nothing(void *data) { (void)data; }
extern struct spinlock multithreading_lock;

void mutex_lock(struct mutex_t * lock_)
{
    if (lock_->locked) {
        lock(&multithreading_lock);
        disable_ints();
        struct thread_t * current_thread = thread_get_current();
        list_push_back(&lock_->locked_threads, current_thread);
        mutex_thread_yield();
    } else {
        lock_->locked = 1;
    }
}

void mutex_unlock(struct mutex_t * lock_)
{
    if (list_size(&lock_->locked_threads) == 0) {
        lock_->locked = 0;
    } else {
        lock(&multithreading_lock);
        struct thread_t* thread = list_top(&lock_->locked_threads)->value;
        list_pop_front(&lock_->locked_threads, nothing);
        list_push_back(&running_threads, thread);
        unlock(&multithreading_lock);
    }
}
