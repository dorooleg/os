#include <backtrace.h>
#include <interruptlib.h>
#include <printf.h>

struct frame
{
	void * previous_frame;
	void * return_addr;
} __attribute__((packed));


void backtrace()
{
	struct frame * current_frame;
	int count;
	__asm__ volatile("movq %%rbp, %0" : "=r" (current_frame));
	count = 0;
	printf("backtrace\n");

	while(current_frame->previous_frame != (void*)0)
	{
		printf("%d\t - 0x%x\n", count, current_frame->return_addr);
		current_frame = current_frame->previous_frame;
		++count;
	}
	printf("end of backtrace (total=%d)\n", count);
}

