.text
.extern handler_irq
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

irq0:
	pushq $32
	jmp irq_common_stub

irq1:
	pushq $33
	jmp irq_common_stub

irq2:
	pushq $34
	jmp irq_common_stub

irq3:
	pushq $35
	jmp irq_common_stub

irq4:
	pushq $36
	jmp irq_common_stub

irq5:
	pushq $37
	jmp irq_common_stub

irq6:
	pushq $38
	jmp irq_common_stub

irq7:
	pushq $39
	jmp irq_common_stub

irq8:
	pushq $40
	jmp irq_common_stub

irq9:
	pushq $41
	jmp irq_common_stub

irq10:
	pushq $42
	jmp irq_common_stub

irq11:
	pushq $43
	jmp irq_common_stub

irq12:
	pushq $44
	jmp irq_common_stub

irq13:
	pushq $45
	jmp irq_common_stub

irq14:
	pushq $46
	jmp irq_common_stub

irq15:
	pushq $47
	jmp irq_common_stub

irq_common_stub:
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
    call handler_irq
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
	addq $0x8, %rsp
    iretq
