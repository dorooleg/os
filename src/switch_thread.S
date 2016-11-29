.code64
.section .text
.globl switch_thread
.globl main_thread_start

main_thread_start:
        popq %rdi
        sti
        callq main_thread
        loop: jmp loop

switch_thread:
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
    
    ret
