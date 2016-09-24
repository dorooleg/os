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
	cli
	push $0
	push $0
	jmp intr_common_stub

intr1:
	cli
	push $0
	push $1
	jmp intr_common_stub

intr2:
	cli
	push $0
	push $2
	jmp intr_common_stub

intr3:
	cli
	push $0
	push $3
	jmp intr_common_stub

intr4:
	cli
	push $0
	push $4
	jmp intr_common_stub

intr5:
	cli
	push $0
	push $5
	jmp intr_common_stub

intr6:
	cli
	push $0
	push $6
	jmp intr_common_stub

intr7:
	cli
	push $0
	push $7
	jmp intr_common_stub

intr8:
	cli
    push $0
	push $8
	jmp intr_common_stub

intr9:
	cli
    push $0
	push $9
	jmp intr_common_stub

intr10:
	cli
    push $0
	push $10
	jmp intr_common_stub

intr11:
	cli
    push $0
	push $11
	jmp intr_common_stub

intr12:
	cli
    push $0
	push $12
	jmp intr_common_stub

intr13:
	cli
    push $0
	push $13
	jmp intr_common_stub

intr14:
	cli
    push $0
	push $14
	jmp intr_common_stub

intr15:
	cli
	push $0
	push $15
	jmp intr_common_stub

intr16:
	cli
	push $0
	push $16
	jmp intr_common_stub

intr17:
	cli
	push $0
	push $17
	jmp intr_common_stub

intr18:
	cli
	push $0
	push $18
	jmp intr_common_stub

intr19:
	cli
	push $0
	push $19
	jmp intr_common_stub

intr20:
	cli
	push $0
	push $20
	jmp intr_common_stub

intr21:
	cli
	push $0
	push $21
	jmp intr_common_stub

intr22:
	cli
	push $0
	push $22
	jmp intr_common_stub

intr23:
	cli
	push $0
	push $23
	jmp intr_common_stub

intr24:
	cli
	push $0
	push $24
	jmp intr_common_stub

intr25:
	cli
	push $0
	push $25
	jmp intr_common_stub

intr26:
	cli
	push $0
	push $26
	jmp intr_common_stub

intr27:
	cli
	push $0
	push $27
	jmp intr_common_stub

intr28:
	cli
	push $0
	push $28
	jmp intr_common_stub

intr29:
	cli
	push $0
	push $29
	jmp intr_common_stub

intr30:
	cli
	push $0
	push $30
	jmp intr_common_stub

intr31:
	cli
	push $0
	push $31
	jmp intr_common_stub

intr_common_stub:
    pushq %rsp
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
    pushfq

	movq	%rsp, %rdi

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
    popq %rsp
	addq $16, %rsp

    iretq
