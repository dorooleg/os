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
	cli
    pushq %rsp
	pushq $32
	jmp irq_common_stub

irq1:
	cli
    pushq %rsp
	pushq $33
	jmp irq_common_stub

irq2:
	cli
    pushq %rsp
	pushq $34
	jmp irq_common_stub

irq3:
	cli
    pushq %rsp
	pushq $35
	jmp irq_common_stub

irq4:
	cli
    pushq %rsp
	pushq $36
	jmp irq_common_stub

irq5:
	cli
    pushq %rsp
	pushq $37
	jmp irq_common_stub

irq6:
	cli
    pushq %rsp
	pushq $38
	jmp irq_common_stub

irq7:
	cli
    pushq %rsp
	pushq $39
	jmp irq_common_stub

irq8:
	cli
    pushq %rsp
	pushq $40
	jmp irq_common_stub

irq9:
	cli
    pushq %rsp
	pushq $41
	jmp irq_common_stub

irq10:
	cli
    pushq %rsp
	pushq $42
	jmp irq_common_stub

irq11:
	cli
    pushq %rsp
	pushq $43
	jmp irq_common_stub

irq12:
	cli
    pushq %rsp
	pushq $44
	jmp irq_common_stub

irq13:
	cli
    pushq %rsp
	pushq $45
	jmp irq_common_stub

irq14:
	cli
    pushq %rsp
	pushq $46
	jmp irq_common_stub

irq15:
	cli
    pushq %rsp
	pushq $47
	jmp irq_common_stub

irq_common_stub:
    #pushq %rsp
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
#    pushfq
	movq	%rsp, %rdi
    call handler_irq
# 	popfq
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
    #popq %rsp
	addq $0x8, %rsp
	popq %rsp
	sti
    iretq
