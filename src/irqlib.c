#include <irqlib.h>
#include <interruptlib.h>
#include <desc.h>
#include <ioport.h>
#include <memory.h>
#include <iolib.h>
#include <ints.h>

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();


void (*irq_routines[16])(struct regs *r) = 
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void irq_set_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}


void irq_remap()
{
    disable_ints(); 

	out8(0x20, 0x11);
    out8(0xA0, 0x11);
    out8(0x21, 0x20);
    out8(0xA1, 0x28);
    out8(0x21, 0x04);
    out8(0xA1, 0x02);
    out8(0x21, 0x01);
    out8(0xA1, 0x01);
	
	enable_ints();
}


void irq_init()
{
	irq_remap();

	idt_set_gate(32, (unsigned long)irq0, KERNEL_CS, 0x8E);  
	idt_set_gate(33, (unsigned long)irq1, KERNEL_CS, 0x8E);  
	idt_set_gate(34, (unsigned long)irq2, KERNEL_CS, 0x8E);  
	idt_set_gate(35, (unsigned long)irq3, KERNEL_CS, 0x8E);  
	idt_set_gate(36, (unsigned long)irq4, KERNEL_CS, 0x8E);  
	idt_set_gate(37, (unsigned long)irq5, KERNEL_CS, 0x8E);  
	idt_set_gate(38, (unsigned long)irq6, KERNEL_CS, 0x8E);  
	idt_set_gate(39, (unsigned long)irq7, KERNEL_CS, 0x8E);  
	idt_set_gate(40, (unsigned long)irq8, KERNEL_CS, 0x8E);  
	idt_set_gate(41, (unsigned long)irq8, KERNEL_CS, 0x8E);  
	idt_set_gate(42, (unsigned long)irq10, KERNEL_CS, 0x8E);  
	idt_set_gate(43, (unsigned long)irq11, KERNEL_CS, 0x8E);  
	idt_set_gate(44, (unsigned long)irq12, KERNEL_CS, 0x8E);  
	idt_set_gate(45, (unsigned long)irq13, KERNEL_CS, 0x8E);  
	idt_set_gate(46, (unsigned long)irq14, KERNEL_CS, 0x8E);  
	idt_set_gate(47, (unsigned long)irq15, KERNEL_CS, 0x8E);  
}

void handler_irq(struct regs *r)
{
    void (*handler)(struct regs *r);

    handler = irq_routines[r->code - 32];
    if (handler)
    {
        handler(r);
    }

    if (r->code >= 40)
    {
        out8(0xA0, 0x20);
    }

    out8(0x20, 0x20);
}
