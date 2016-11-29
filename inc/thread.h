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
    void            *stack_head;
    void*           (*start_routine)(void*);
    void            *result;
    void            *arg;
    uint64_t        stack_size; 
    thread_status   status;
    uint64_t        tid;
} __attribute__((packed));

struct thread_frame
{
    uint64_t rflags;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t rbp;
    uint64_t rbx;
    uint64_t rdi;
    uint64_t rip;
} __attribute__((packed));

void threads_init();

struct thread_t* thread_create(void *(*start_routine)(void *), void *arg);

void thread_start(struct thread_t* thread);

void thread_destroy(struct thread_t* thread);

void thread_join(struct thread_t* thread, void** result);

void thread_yield();

void thread_yield_interrupt();

void mutex_thread_yield();

struct thread_t* thread_get_current();

void print_current_thread();

void print_init_threads();

void print_running_threads();

void print_terminated_threads();

void main_thread(struct thread_t* thread);

#endif //__THREAD_H__

