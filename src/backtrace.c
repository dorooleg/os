#include <backtrace.h>
#include <interruptlib.h>
#include <printf.h>
#include <memory.h>

struct frame
{
	void * previous_frame;
	void * return_addr;
} __attribute__((packed));


void backtrace()
{
    void * sp;
	struct frame * current_frame;
	int count;
	__asm__ volatile("movq %%rbp, %0" : "=r" (current_frame));
	__asm__ volatile("movq %%rsp, %0" : "=r" (sp));
	count = 0;
	printf("backtrace\n");

	if ((void*)current_frame >= sp
          && ((uint64_t)current_frame & (uint64_t)VIRTUAL_BASE) == (uint64_t)VIRTUAL_BASE
          && ((uint64_t)current_frame & (uint64_t)0xffffffff) <= (uint64_t)0x80107000)
    {
        while(current_frame->previous_frame >= sp
              && ((uint64_t)current_frame->previous_frame & (uint64_t)VIRTUAL_BASE) == (uint64_t)VIRTUAL_BASE
              && ((uint64_t)current_frame->previous_frame & (uint64_t)0xffffffff) <= (uint64_t)0x80107000)
        {
            printf("%d\t - 0x%p\n", count, current_frame->return_addr);
            ++count;
            if ((void*)current_frame >= current_frame->previous_frame) {
                break;
            }
            current_frame = current_frame->previous_frame;
        }
    }
	printf("end of backtrace (total=%d)\n", count);
}

