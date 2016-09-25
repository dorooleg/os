#ifndef __INTERRUPTLIB_H__
#define __INTERRUPTLIB_H__

#include <desc.h>

#define interrupt(x) { __asm__ volatile("int %0" : : "n"(x)); }

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void handler_interrupt(struct regs * r);
void intr_install();
void idt_install();

#endif /*__INTERRUPTLIB_H__*/
