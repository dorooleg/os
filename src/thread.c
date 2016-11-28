#include <thread.h>
#include <list.h>
#include <memory.h>
#include <spinlock.h>
#include <fast_slab.h>
#include <printf.h>
#include <pagea.h>
#include <interruptlib.h>
#include <ints.h>
#include <pitlib.h>
#include <ioport.h>

uint64_t gtid;

struct spinlock multithreading_lock;

static list_t * init_threads = NULL;
list_t * running_threads = NULL;
static list_t * terminated_threads = NULL;
static struct thread_t* volatile current_thread = NULL;
fast_slab_metadata thread_t_allocator;  

static void nothing(void *data) { (void)data; }

static uint64_t p_tid;
static char predicate_tid(void* value)
{
    struct thread_t * thread = value;
    if (thread->tid == p_tid) {
        return 1;
    }
    return 0;
}

int counter_threads;
void print_thread(void* value)
{
    struct thread_t *thread = value;
    printf("%i: ", counter_threads++);
    printf("TID = %lli, SP = %p, STATUS = ", thread->tid, thread->sp);
    switch (thread->status)
    {
        case INIT:
            printf("init");
            break;
        case TERMINATED:
            printf("terminated");
            break;
        case RUNNING:
            printf("running");
            break;
    }
    printf("\n");
}

void threads_init()
{
    thread_t_allocator = create_fast_slab_allocator_concurrent(sizeof(struct thread_t));
    current_thread = thread_create(NULL, NULL);
    current_thread->status = RUNNING;
}

struct thread_t* thread_create(void *(*start_routine)(void *), void *arg)
{
    lock(&multithreading_lock);
    struct thread_t * thread = alloc_fast_slab_concurrent(&thread_t_allocator);
    thread->stack_head = pagea_alloc_concurrent(THREAD_MAX_STACK_SIZE);
    thread->sp = (uint8_t*)thread->stack_head + THREAD_MAX_STACK_SIZE * PAGE_SIZE - 1;
    thread->sp = (uint8_t*)thread->sp - sizeof(struct thread_frame);
    struct thread_frame * frame = thread->sp;
    frame->rflags = 0;
    frame->r15 = 0;
    frame->r13 = 0;
    frame->r12 = 0;
    frame->rbp = 0;
    frame->rbx = 0;
    frame->rdi = (uint64_t)thread;
    frame->rip = (uint64_t)main_thread;
    thread->stack_size = THREAD_MAX_STACK_SIZE;
    thread->start_routine = start_routine;
    thread->arg = arg;
    thread->result = 0;
    thread->status = INIT;
    thread->tid = gtid++;
    list_push_front(&init_threads, thread);
    unlock(&multithreading_lock);
    return thread;
}

void thread_terminate()
{
    lock(&multithreading_lock);
    current_thread->status = TERMINATED;
    list_push_back(&terminated_threads, current_thread);
    p_tid = current_thread->tid;
    list_remove_first(&running_threads, predicate_tid, nothing);
    unlock(&multithreading_lock);
    thread_yield();
}

void thread_start(struct thread_t* thread)
{
    lock(&multithreading_lock);
    list_push_back(&running_threads, thread);
    p_tid = thread->tid;
    list_remove_first(&init_threads, predicate_tid, nothing);
    unlock(&multithreading_lock);
}

void thread_join(struct thread_t* thread, void** result)
{
    while (thread->status != TERMINATED) {
        thread_yield();
        __asm__ volatile ("" : : : "memory");
    }
    *result = thread->result;
}


void switch_thread(void **prev, void *next);

void mutex_thread_yield()
{
    lock(&multithreading_lock);
    if (current_thread != NULL) {
        struct thread_t* thread = list_top(&running_threads)->value;
        list_pop_front(&running_threads, nothing);
        unlock(&multithreading_lock);
        struct thread_t * old_thread = current_thread;
        current_thread = thread;
        switch_thread(&old_thread->sp, &thread->sp);
    }
}

void thread_yield()
{
    lock(&multithreading_lock);
    if (current_thread != NULL && current_thread->status == RUNNING) {
        list_push_back(&running_threads, current_thread);
    }

    if (current_thread != NULL) {
        struct thread_t* thread = list_top(&running_threads)->value;
        list_pop_front(&running_threads, nothing);
        struct thread_t * old_thread = current_thread;
        current_thread = thread;
        unlock(&multithreading_lock);
        disable_ints();
        switch_thread(&old_thread->sp, &thread->sp);
    }
    else
    {
        unlock(&multithreading_lock);
    }
}

void thread_destroy(struct thread_t* thread)
{
    lock(&multithreading_lock);
    pagea_free_concurrent(thread->sp);
    p_tid = thread->tid;
    list_remove_first(&terminated_threads, predicate_tid, nothing);
    list_remove_first(&running_threads, predicate_tid, nothing);
    unlock(&multithreading_lock);
}

void main_thread(struct thread_t* thread)
{
    if (thread->status == INIT)
    {
        thread->status = RUNNING;
        thread_yield();
        thread->result = thread->start_routine(thread->arg);
        thread_terminate();
    }
}

struct thread_t* thread_get_current()
{
    return current_thread;
}

void print_current_thread()
{
    printf("Current thread\n");
    print_thread(current_thread);
}

void print_init_threads()
{
    printf("Init threads\n");
    counter_threads = 0;
    list_map(&init_threads, print_thread);    
}

void print_running_threads()
{
    printf("Running threads\n");
    counter_threads = 0;
    list_map(&running_threads, print_thread);    
}

void print_terminated_threads()
{
    printf("Terminated threads\n");
    counter_threads = 0;
    list_map(&terminated_threads, print_thread);    
}
