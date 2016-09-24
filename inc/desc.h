#ifndef __DESC_H__
#define __DESC_H__

#include <stdint.h>

struct regs {
	uint64_t fq;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
   	uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rsp;
	unsigned char code;
	unsigned char empty;
} __attribute__((packed));

struct idt_entry {
   uint16_t base_lo;
   uint16_t sel;
   uint8_t  always0;
   uint8_t  flags;
   uint16_t base_hi;
   uint32_t base_64;
   uint32_t reserved;
} __attribute__((packed));

struct desc_table_ptr {
	uint16_t size;
	uint64_t addr;
} __attribute__((packed));

static inline void read_idtr(struct desc_table_ptr *ptr)
{
	__asm__ ("sidt %0" : "=m"(*ptr));
}

static inline void write_idtr(const struct desc_table_ptr *ptr)
{
	__asm__ ("lidt %0" : : "m"(*ptr));
}

static inline void read_gdtr(struct desc_table_ptr *ptr)
{
	__asm__ ("sgdt %0" : "=m"(*ptr));
}

static inline void write_gdtr(const struct desc_table_ptr *ptr)
{
	__asm__ ("lgdt %0" : : "m"(*ptr));
}

#endif /*__DESC_H__*/
