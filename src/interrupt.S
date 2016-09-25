.text
.extern handler_interrupt
.global intr0
.global intr1
.global intr2
.global intr3
.global intr4
.global intr5
.global intr6
.global intr7
.global intr8
.global intr9
.global intr10
.global intr11
.global intr12
.global intr13
.global intr14
.global intr15
.global intr16
.global intr17
.global intr18
.global intr19
.global intr20
.global intr21
.global intr22
.global intr23
.global intr24
.global intr25
.global intr26
.global intr27
.global intr28
.global intr29
.global intr30
.global intr31

intr0:
	subq $8, %rsp
	pushq $0
	jmp intr_common_stub

intr1:
	subq $8, %rsp
	pushq $1
	jmp intr_common_stub

intr2:
	subq $8, %rsp
	pushq $2
	jmp intr_common_stub

intr3:
	subq $8, %rsp
	pushq $3
	jmp intr_common_stub

intr4:
	subq $8, %rsp
	pushq $8
	jmp intr_common_stub

intr5:
	subq $8, %rsp
	pushq $5
	jmp intr_common_stub

intr6:
	subq $8, %rsp
	pushq $6
	jmp intr_common_stub

intr7:
	subq $8, %rsp
	pushq $7
	jmp intr_common_stub

intr8:
	subq $8, %rsp
	pushq $8
	jmp intr_common_stub

intr9:
	subq $8, %rsp
	pushq $9
	jmp intr_common_stub

intr10:
	pushq $10
	jmp intr_common_stub

intr11:
	pushq $11
	jmp intr_common_stub

intr12:
	pushq $12
	jmp intr_common_stub

intr13:
	pushq $13
	jmp intr_common_stub

intr14:
	pushq $14
	jmp intr_common_stub

intr15:
	subq $8, %rsp
	pushq $15
	jmp intr_common_stub

intr16:
	subq $8, %rsp
	pushq $16
	jmp intr_common_stub

intr17:
	subq $8, %rsp
	pushq $17
	jmp intr_common_stub

intr18:
	subq $8, %rsp
	pushq $18
	jmp intr_common_stub

intr19:
	subq $8, %rsp
	pushq $19
	jmp intr_common_stub

intr20:
	subq $8, %rsp
	pushq $20
	jmp intr_common_stub

intr21:
	subq $8, %rsp
	pushq $21
	jmp intr_common_stub

intr22:
	subq $8, %rsp
	pushq $22
	jmp intr_common_stub

intr23:
	subq $8, %rsp
	pushq $23
	jmp intr_common_stub

intr24:
	subq $8, %rsp
	pushq $24
	jmp intr_common_stub

intr25:
	subq $8, %rsp
	pushq $25
	jmp intr_common_stub

intr26:
	subq $8, %rsp
	pushq $26
	jmp intr_common_stub

intr27:
	subq $8, %rsp
	pushq $27
	jmp intr_common_stub

intr28:
	subq $8, %rsp
	pushq $28
	jmp intr_common_stub

intr29:
	subq $8, %rsp
	pushq $29
	jmp intr_common_stub

intr30:
	subq $8, %rsp
	jmp intr_common_stub

intr31:
	subq $8, %rsp
	jmp intr_common_stub

intr_common_stub:
	pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
	movq	%rsp, %rdi
    pushfq
    cld
    call handler_interrupt
	popfq
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
	addq $0x10, %rsp
    iretq
