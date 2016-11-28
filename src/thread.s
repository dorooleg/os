	.file	"thread.c"
	.comm	gtid,8,8
	.comm	multithreading_lock,4,4
	.local	init_threads
	.comm	init_threads,8,8
	.globl	running_threads
	.bss
	.align 8
	.type	running_threads, @object
	.size	running_threads, 8
running_threads:
	.zero	8
	.local	terminated_threads
	.comm	terminated_threads,8,8
	.local	current_thread
	.comm	current_thread,8,8
	.local	current_sp
	.comm	current_sp,8,8
	.comm	thread_t_allocator,24,16
	.text
	.type	nothing, @function
nothing:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	nothing, .-nothing
	.local	p_tid
	.comm	p_tid,8,8
	.type	predicate_tid, @function
predicate_tid:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	56(%rax), %rdx
	movq	p_tid(%rip), %rax
	cmpq	%rax, %rdx
	jne	.L3
	movl	$1, %eax
	jmp	.L4
.L3:
	movl	$0, %eax
.L4:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	predicate_tid, .-predicate_tid
	.comm	counter_threads,4,4
	.section	.rodata
.LC0:
	.string	"%i: "
	.align 8
.LC1:
	.string	"TID = %lli, SP = %p, STATUS = "
.LC2:
	.string	"init"
.LC3:
	.string	"terminated"
.LC4:
	.string	"running"
	.text
	.globl	print_thread
	.type	print_thread, @function
print_thread:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movl	counter_threads(%rip), %eax
	leal	1(%rax), %edx
	movl	%edx, counter_threads(%rip)
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movq	-8(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	56(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movq	-8(%rbp), %rax
	movl	48(%rax), %eax
	cmpl	$1, %eax
	je	.L7
	cmpl	$1, %eax
	jb	.L8
	cmpl	$2, %eax
	je	.L9
	jmp	.L6
.L8:
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	jmp	.L6
.L9:
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	jmp	.L6
.L7:
	movl	$.LC4, %edi
	movl	$0, %eax
	call	printf
	nop
.L6:
	movl	$10, %edi
	call	putchar
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	print_thread, .-print_thread
	.globl	threads_init
	.type	threads_init, @function
threads_init:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-48(%rbp), %rax
	movl	$64, %esi
	movq	%rax, %rdi
	call	create_fast_slab_allocator_concurrent
	movq	-48(%rbp), %rax
	movq	%rax, thread_t_allocator(%rip)
	movq	-40(%rbp), %rax
	movq	%rax, thread_t_allocator+8(%rip)
	movq	-32(%rbp), %rax
	movq	%rax, thread_t_allocator+16(%rip)
	movl	$0, %esi
	movl	$0, %edi
	call	thread_create
	movq	%rax, current_thread(%rip)
	movq	current_thread(%rip), %rax
	movl	$1, 48(%rax)
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L11
	call	__stack_chk_fail
.L11:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	threads_init, .-threads_init
	.section	.rodata
.LC5:
	.string	"THREAD = %p"
	.text
	.globl	thread_create
	.type	thread_create, @function
thread_create:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$multithreading_lock, %edi
	call	lock
	movl	$thread_t_allocator, %edi
	call	alloc_fast_slab_concurrent
	movq	%rax, -16(%rbp)
	movl	$1, %edi
	call	pagea_alloc_concurrent
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-16(%rbp), %rax
	movq	8(%rax), %rax
	leaq	4095(%rax), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	leaq	-80(%rax), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC5, %edi
	movl	$0, %eax
	call	printf
	movq	-8(%rbp), %rax
	movq	$0, (%rax)
	movq	-8(%rbp), %rax
	movq	$0, 8(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 24(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 32(%rax)
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 40(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 48(%rax)
	movl	$main_thread, %edx
	movq	-8(%rbp), %rax
	movq	%rdx, 56(%rax)
	movq	-16(%rbp), %rax
	movq	$1, 40(%rax)
	movq	-16(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 16(%rax)
	movq	-16(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, 32(%rax)
	movq	-16(%rbp), %rax
	movq	$0, 24(%rax)
	movq	-16(%rbp), %rax
	movl	$0, 48(%rax)
	movq	gtid(%rip), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, gtid(%rip)
	movq	-16(%rbp), %rdx
	movq	%rax, 56(%rdx)
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$init_threads, %edi
	call	list_push_front
	movl	$multithreading_lock, %edi
	call	unlock
	movq	-16(%rbp), %rax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	thread_create, .-thread_create
	.globl	thread_terminate
	.type	thread_terminate, @function
thread_terminate:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$multithreading_lock, %edi
	call	lock
	movq	current_thread(%rip), %rax
	movl	$2, 48(%rax)
	movq	current_thread(%rip), %rax
	movq	%rax, %rsi
	movl	$terminated_threads, %edi
	call	list_push_back
	movq	current_thread(%rip), %rax
	movq	56(%rax), %rax
	movq	%rax, p_tid(%rip)
	movl	$nothing, %edx
	movl	$predicate_tid, %esi
	movl	$running_threads, %edi
	call	list_remove_first
	movl	$multithreading_lock, %edi
	call	unlock
	movl	$0, %eax
	call	thread_yield
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	thread_terminate, .-thread_terminate
	.globl	thread_start
	.type	thread_start, @function
thread_start:
.LFB12:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	$multithreading_lock, %edi
	call	lock
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$running_threads, %edi
	call	list_push_back
	movq	-8(%rbp), %rax
	movq	56(%rax), %rax
	movq	%rax, p_tid(%rip)
	movl	$nothing, %edx
	movl	$predicate_tid, %esi
	movl	$init_threads, %edi
	call	list_remove_first
	movl	$multithreading_lock, %edi
	call	unlock
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	thread_start, .-thread_start
	.globl	thread_join
	.type	thread_join, @function
thread_join:
.LFB13:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	jmp	.L17
.L18:
	movl	$0, %eax
	call	thread_yield
.L17:
	movq	-8(%rbp), %rax
	movl	48(%rax), %eax
	cmpl	$2, %eax
	jne	.L18
	movq	-8(%rbp), %rax
	movq	24(%rax), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, (%rax)
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	thread_join, .-thread_join
	.globl	mutex_thread_yield
	.type	mutex_thread_yield, @function
mutex_thread_yield:
.LFB14:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$multithreading_lock, %edi
	call	lock
	movq	current_thread(%rip), %rax
	testq	%rax, %rax
	je	.L21
	movl	$running_threads, %edi
	call	list_top
	movq	8(%rax), %rax
	movq	%rax, -8(%rbp)
	movl	$nothing, %esi
	movl	$running_threads, %edi
	call	list_pop_front
	movl	$multithreading_lock, %edi
	call	unlock
	movq	-8(%rbp), %rax
	movq	%rax, current_thread(%rip)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$current_sp, %edi
	call	switch_thread
.L21:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	mutex_thread_yield, .-mutex_thread_yield
	.globl	thread_yield
	.type	thread_yield, @function
thread_yield:
.LFB15:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$multithreading_lock, %edi
	call	lock
	movq	current_thread(%rip), %rax
	testq	%rax, %rax
	je	.L23
	movq	current_thread(%rip), %rax
	movl	48(%rax), %eax
	cmpl	$1, %eax
	jne	.L23
	movq	current_thread(%rip), %rax
	movq	%rax, %rsi
	movl	$running_threads, %edi
	call	list_push_back
.L23:
	movq	current_thread(%rip), %rax
	testq	%rax, %rax
	je	.L24
	movl	$running_threads, %edi
	call	list_top
	movq	8(%rax), %rax
	movq	%rax, -8(%rbp)
	movl	$nothing, %esi
	movl	$running_threads, %edi
	call	list_pop_front
	movl	$multithreading_lock, %edi
	call	unlock
	movq	-8(%rbp), %rax
	movq	%rax, current_thread(%rip)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$current_sp, %edi
	call	switch_thread
	jmp	.L26
.L24:
	movl	$multithreading_lock, %edi
	call	unlock
.L26:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	thread_yield, .-thread_yield
	.globl	thread_destroy
	.type	thread_destroy, @function
thread_destroy:
.LFB16:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	$multithreading_lock, %edi
	call	lock
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	pagea_free_concurrent
	movq	-8(%rbp), %rax
	movq	56(%rax), %rax
	movq	%rax, p_tid(%rip)
	movl	$nothing, %edx
	movl	$predicate_tid, %esi
	movl	$terminated_threads, %edi
	call	list_remove_first
	movl	$nothing, %edx
	movl	$predicate_tid, %esi
	movl	$running_threads, %edi
	call	list_remove_first
	movl	$multithreading_lock, %edi
	call	unlock
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	thread_destroy, .-thread_destroy
	.section	.rodata
.LC6:
	.string	"MAIN!"
.LC7:
	.string	"MAIN! %p %p %p\n"
	.text
	.globl	main_thread
	.type	main_thread, @function
main_thread:
.LFB17:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
#APP
# 171 "thread.c" 1
	movq %rsp, %rax
# 0 "" 2
#NO_APP
	movq	%rax, -16(%rbp)
#APP
# 173 "thread.c" 1
	movq %rbp, %rax
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movl	$.LC6, %edi
	movl	$0, %eax
	call	printf
	movq	-8(%rbp), %rcx
	movq	-16(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	movq	-24(%rbp), %rax
	movl	48(%rax), %eax
	testl	%eax, %eax
	jne	.L30
	movq	-24(%rbp), %rax
	movl	$1, 48(%rax)
	movq	-24(%rbp), %rax
	movq	16(%rax), %rax
	movq	-24(%rbp), %rdx
	movq	32(%rdx), %rdx
	movq	%rdx, %rdi
	call	*%rax
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, 24(%rax)
	movl	$0, %eax
	call	thread_terminate
.L30:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	main_thread, .-main_thread
	.globl	thread_get_current
	.type	thread_get_current, @function
thread_get_current:
.LFB18:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	current_thread(%rip), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	thread_get_current, .-thread_get_current
	.section	.rodata
.LC8:
	.string	"Current thread"
	.text
	.globl	print_current_thread
	.type	print_current_thread, @function
print_current_thread:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC8, %edi
	call	puts
	movq	current_thread(%rip), %rax
	movq	%rax, %rdi
	call	print_thread
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	print_current_thread, .-print_current_thread
	.section	.rodata
.LC9:
	.string	"Init threads"
	.text
	.globl	print_init_threads
	.type	print_init_threads, @function
print_init_threads:
.LFB20:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC9, %edi
	call	puts
	movl	$0, counter_threads(%rip)
	movl	$print_thread, %esi
	movl	$init_threads, %edi
	call	list_map
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	print_init_threads, .-print_init_threads
	.section	.rodata
.LC10:
	.string	"Running threads"
	.text
	.globl	print_running_threads
	.type	print_running_threads, @function
print_running_threads:
.LFB21:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC10, %edi
	call	puts
	movl	$0, counter_threads(%rip)
	movl	$print_thread, %esi
	movl	$running_threads, %edi
	call	list_map
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE21:
	.size	print_running_threads, .-print_running_threads
	.section	.rodata
.LC11:
	.string	"Terminated threads"
	.text
	.globl	print_terminated_threads
	.type	print_terminated_threads, @function
print_terminated_threads:
.LFB22:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC11, %edi
	call	puts
	movl	$0, counter_threads(%rip)
	movl	$print_thread, %esi
	movl	$terminated_threads, %edi
	call	list_map
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	print_terminated_threads, .-print_terminated_threads
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
