#include <mutex.h>
#include <ints.h>

extern list_t * running_threads;
static void nothing(void *data) { (void)data; }

void mutex_lock(struct mutex_t * lock)
{
    disable_ints();
    if (lock->locked) {
        struct thread_t * current_thread = thread_get_current();
        list_push_back(&lock->locked_threads, current_thread);
        mutex_thread_yield();
    } else {
        lock->locked = 1;
    }
    enable_ints();
}

void mutex_unlock(struct mutex_t * lock)
{
    disable_ints();
    if (list_size(&lock->locked_threads) == 0) {
        lock->locked = 0;
    } else {
        struct thread_t* thread = list_top(&lock->locked_threads)->value;
        list_pop_front(&lock->locked_threads, nothing);
        list_push_back(&running_threads, thread);
    }
    enable_ints();
}
