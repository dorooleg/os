#include <iolib.h>
#include <ioport.h>

void init_io()
{
    //disable interrupt 
	out8(0x3f8 + 3, 0); 
	out8(0x3f8 + 1, 0); 

    //delimiter 12
	out8(0x3f8 + 3, 0xf0); 
	out8(0x3f8 + 0, 12); 

    //8 bit in frame
	out8(0x3f8 + 3, 0x3); 
}

void write(const char * const str)
{
    uint32_t i = 0;
    while (str[i]) {
       while (!(in8(0x3f8 + 5) & 0x20));
       out8(0x3f8, str[i]); 
       i++;
    }
}
