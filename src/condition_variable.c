#include <condition_variable.h>
#include <lock.h>
#include <ints.h>

extern list_t * running_threads;
static void nothing(void *data) { (void)data; }
extern struct spinlock multithreading_lock;

void cond_wait(struct cond_t * cond, struct mutex_t * lock)
{
    locki();
    struct thread_t * current_thread = thread_get_current();
    list_push_back(&cond->locked_threads, current_thread);
    cond_thread_yield(lock);
}

void cond_signal(struct cond_t * cond)
{
    locki();
    if (list_size(&cond->locked_threads) != 0) {
        struct thread_t* thread = list_top(&cond->locked_threads)->value;
        list_pop_front(&cond->locked_threads, nothing);
        list_push_back(&running_threads, thread);
    }
    unlocki();
}

void cond_broadcast(struct cond_t * cond)
{
    locki();
    while (list_size(&cond->locked_threads) != 0) {
        struct thread_t* thread = list_top(&cond->locked_threads)->value;
        list_pop_front(&cond->locked_threads, nothing);
        list_push_back(&running_threads, thread);
    }
    unlocki();
}
