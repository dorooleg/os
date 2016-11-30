#ifndef __INTS_H__
#define __INTS_H__
#include <printf.h>

static inline void disable_ints(void)
{ 
    __asm__ volatile ("cli" : : : "cc"); 
}

static inline void enable_ints(void)
{ 
    __asm__ volatile ("sti" : : : "cc"); 
}

#endif /*__INTS_H__*/
