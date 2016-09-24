#include <irqlib.h>
#include <iolib.h>
#include <ioport.h>
#include <pitlib.h>

void pit_init()
{
    irq_set_handler(0, timer_handler);
}

void timer_handler(struct regs *r)
{
	(void)r;
    write("Timer call\n");
}

void timer_init()
{
	int divisor = 1193180;    /* Calculate our divisor */
    out8(0x43, 0x36);             /* Set our command byte 0x36 */
    out8(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    out8(0x40, (divisor >> 8) & 0xFF);     /* Set high byte of divisor */
}
