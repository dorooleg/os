.code64
.section .text
.global switch_thread

switch_thread:
    pushq %rdi
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    pushfq

    mov %rsp, (%rdi)
    mov (%rsi), %rsp
    
    popfq
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    popq %rdi
    sti
    
    ret
