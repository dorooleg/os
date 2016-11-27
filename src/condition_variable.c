#include <condition_variable.h>
#include <ints.h>

extern list_t * running_threads;
static void nothing(void *data) { (void)data; }

void cond_wait(struct cond_t * lock)
{
    disable_ints();
    struct thread_t * current_thread = thread_get_current();
    list_push_back(&lock->locked_threads, current_thread);
    mutex_thread_yield();
    enable_ints();
}

void cond_signal(struct cond_t * lock)
{
    disable_ints();
    if (list_size(&lock->locked_threads) != 0) {
        struct thread_t* thread = list_top(&lock->locked_threads)->value;
        list_pop_front(&lock->locked_threads, nothing);
        list_push_back(&running_threads, thread);
    }
    enable_ints();
}

void cond_broadcast(struct cond_t * lock)
{
    while (list_size(&lock->locked_threads) != 0) {
        struct thread_t* thread = list_top(&lock->locked_threads)->value;
        list_pop_front(&lock->locked_threads, nothing);
        list_push_back(&running_threads, thread);
    }
}
