#ifndef __PITLIB_H__
#define __PITLIB_H__

#include <desc.h>

void pit_init();
void timer_handler(struct regs *r);
void timer_init();

#endif /*__PITLIB_H__*/
