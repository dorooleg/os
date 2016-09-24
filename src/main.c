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

void main(void)
{
    qemu_gdb_hang();

    struct desc_table_ptr ptr = {0, 0};

    write_idtr(&ptr);

    init_io();
    idt_install();
    intr_install();

    interrupt(8);

    while (1);
}
