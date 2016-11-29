#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include <stdatomic.h>

#define LOCKED 1
#define UNLOCKED 0

struct spinlock
{
	atomic_int locked;
};

void lock(struct spinlock * lock);
char try_lock(struct spinlock * lock);
void unlock(struct spinlock * lock);

#endif //__SPINLOCK_H__
