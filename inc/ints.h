#ifndef __INTS_H__
#define __INTS_H__
#include <printf.h>

int cnt;
static inline void disable_ints(void)
{ 
    __asm__ volatile ("cli" : : : "cc"); 
    cnt++;
}

static inline void enable_ints(void)
{ 
    cnt--;
    if (cnt == 0)
        __asm__ volatile ("sti" : : : "cc"); 
}

#endif /*__INTS_H__*/
