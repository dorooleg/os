#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <stdatomic.h>
#include <thread.h>
#include <list.h>

struct mutex_t
{
	atomic_int locked;
    list_t * locked_threads;
};

void mutex_lock(struct mutex_t * lock);
void mutex_unlock(struct mutex_t * lock);

#endif //__MUTEX_H__
