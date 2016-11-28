#include <irqlib.h>
#include <iolib.h>
#include <ioport.h>
#include <pitlib.h>
#include <thread.h>
#include <ints.h>

void pit_init()
{
    irq_set_handler(0, timer_handler);
}

void timer_handler(struct regs *r)
{
    (void)r;
//    write("TIMER\n");
    if (r->code >= 40)
    {
        out8(0xA0, 0x20);
    }

    out8(0x20, 0x20);
    thread_yield();
}

void timer_init()
{
    int divisor = 1193180;    /* Calculate our divisor */
    out8(0x43, 0x36);             /* Set our command byte 0x36 */
    out8(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    out8(0x40, (divisor >> 8) & 0xFF);     /* Set high byte of divisor */
}
