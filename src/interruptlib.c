#include <interruptlib.h>
#include <iolib.h>
#include <desc.h>
#include <memory.h>

extern void intr0();
extern void intr1();
extern void intr2();
extern void intr3();
extern void intr4();
extern void intr5();
extern void intr6();
extern void intr7();
extern void intr8();
extern void intr9();
extern void intr10();
extern void intr11();
extern void intr12();
extern void intr13();
extern void intr14();
extern void intr15();
extern void intr16();
extern void intr17();
extern void intr18();
extern void intr19();
extern void intr20();
extern void intr21();
extern void intr22();
extern void intr23();
extern void intr24();
extern void intr25();
extern void intr26();
extern void intr27();
extern void intr28();
extern void intr29();
extern void intr30();
extern void intr31();

char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void handler_interrupt(struct regs * r)
{
	write("Hello world!");
	if (r->code < 32) {
		write(exception_messages[r->code]);
	}
}

struct idt_entry idt[256];
struct desc_table_ptr idtp = { 0, 0 };

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    /* The interrupt routine's base address */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
	idt[num].base_64 = (base >> 32) & 0xFFFFFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Installs the IDT */
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.size = (sizeof (struct idt_entry) * 31) - 1;
    idtp.addr = (uint64_t)idt;

    /* Clear out the entire IDT, initializing it to zeros */
	for (uint64_t i = 0; i < sizeof(struct idt_entry) * 31; i++) {
		((char*)idt)[i] = 0;
	}

    /* Add any new ISRs to the IDT here using idt_set_gate */


    /* Points the processor's internal register to the new IDT */
	write_idtr(&idtp);
}

void intr_install()
{
	idt_set_gate(0, (unsigned long)intr0, KERNEL_CS, 0x8E);  
	idt_set_gate(1, (unsigned long)intr1, KERNEL_CS, 0x8E);  
	idt_set_gate(2, (unsigned long)intr2, KERNEL_CS, 0x8E);  
	idt_set_gate(3, (unsigned long)intr3, KERNEL_CS, 0x8E);  
	idt_set_gate(4, (unsigned long)intr4, KERNEL_CS, 0x8E);  
	idt_set_gate(5, (unsigned long)intr5, KERNEL_CS, 0x8E);  
	idt_set_gate(6, (unsigned long)intr6, KERNEL_CS, 0x8E);  
	idt_set_gate(7, (unsigned long)intr7, KERNEL_CS, 0x8E);  
	idt_set_gate(8, (unsigned long)intr8, KERNEL_CS, 0x8E);  
	idt_set_gate(9, (unsigned long)intr9, KERNEL_CS, 0x8E);  
	idt_set_gate(10, (unsigned long)intr10, KERNEL_CS, 0x8E);  
	idt_set_gate(11, (unsigned long)intr11, KERNEL_CS, 0x8E);  
	idt_set_gate(12, (unsigned long)intr12, KERNEL_CS, 0x8E);  
	idt_set_gate(13, (unsigned long)intr13, KERNEL_CS, 0x8E);  
	idt_set_gate(14, (unsigned long)intr14, KERNEL_CS, 0x8E);  
	idt_set_gate(15, (unsigned long)intr15, KERNEL_CS, 0x8E);  
	idt_set_gate(16, (unsigned long)intr16, KERNEL_CS, 0x8E);  
	idt_set_gate(17, (unsigned long)intr17, KERNEL_CS, 0x8E);  
	idt_set_gate(18, (unsigned long)intr18, KERNEL_CS, 0x8E);  
	idt_set_gate(19, (unsigned long)intr19, KERNEL_CS, 0x8E);  
	idt_set_gate(20, (unsigned long)intr20, KERNEL_CS, 0x8E);  
	idt_set_gate(21, (unsigned long)intr21, KERNEL_CS, 0x8E);  
	idt_set_gate(22, (unsigned long)intr22, KERNEL_CS, 0x8E);  
	idt_set_gate(23, (unsigned long)intr23, KERNEL_CS, 0x8E);  
	idt_set_gate(24, (unsigned long)intr24, KERNEL_CS, 0x8E);  
	idt_set_gate(25, (unsigned long)intr25, KERNEL_CS, 0x8E);  
	idt_set_gate(26, (unsigned long)intr26, KERNEL_CS, 0x8E);  
	idt_set_gate(27, (unsigned long)intr27, KERNEL_CS, 0x8E);  
	idt_set_gate(28, (unsigned long)intr28, KERNEL_CS, 0x8E);  
	idt_set_gate(29, (unsigned long)intr29, KERNEL_CS, 0x8E);  
	idt_set_gate(30, (unsigned long)intr30, KERNEL_CS, 0x8E);  
	idt_set_gate(31, (unsigned long)intr31, KERNEL_CS, 0x8E);  
}
