#ifndef __THREAD_H__
#define __THREAD_H__

#define THREAD_MAX_STACK_SIZE 1 //page size

#include <stdint.h>

typedef enum
{
    INIT,
    RUNNING,
    TERMINATED
} thread_status;


struct thread_t
{
    void            *sp;
    void*           (*start_routine)(void*);
    void            *result;
    void            *arg;
    uint64_t        stack_size; 
    thread_status   status;
    uint64_t        tid;
};

void threads_init();

struct thread_t* thread_create(void *(*start_routine)(void *), void *arg);

void thread_start(struct thread_t* thread);

void thread_destroy(struct thread_t* thread);

void thread_join(struct thread_t* thread, void** result);

void thread_yield();

struct thread_t* thread_get_current();

void print_current_thread();

void print_init_threads();

void print_running_threads();

void print_terminated_threads();

#endif //__THREAD_H__

