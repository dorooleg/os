#ifndef __IRQLIB_H__
#define __IRQLIB_H__

#include <desc.h>
void irq_set_handler(int irq, void (*handler)(struct regs *r));
void irq_remap();
void irq_init();
void irq_handler(struct regs *r);

#endif /*__IRQLIB_H__*/
