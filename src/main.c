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
#include <printf.h>
#include <backtrace.h>

void foo2();
void foo1()
{
    foo2();
}

void foo2()
{
    backtrace();
}

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

    foo1();
    printf("Hello %corld%c%s %o %lli", 'w', '!', "One", 15, 213278569235LL);
    clear_interrupt();

    while (1);
}
