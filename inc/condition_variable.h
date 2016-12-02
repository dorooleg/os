#ifndef __CONDITION_VARIABLE_H__
#define __CONDITION_VARIABLE_H__

#include <stdatomic.h>
#include <thread.h>
#include <mutex.h>
#include <list.h>

struct cond_t
{
    list_t * locked_threads;
};

void cond_wait(struct cond_t * lock, struct mutex_t * lock_);
void cond_signal(struct cond_t * lock);
void cond_broadcast(struct cond_t * lock);

#endif //__CONDITION_VARIABLE_H__
