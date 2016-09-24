//#define DEBUG
static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include <desc.h>
#include <ints.h>
#include <iolib.h>
#include <interruptlib.h>
#include <pitlib.h>
#include <irqlib.h>
#include <ioport.h>

void main(void)
{
    qemu_gdb_hang();

    init_io();
    idt_install();
    intr_install();
    irq_init();
    pit_init();
    timer_init();

	interrupt(1);



    while (1);
}
