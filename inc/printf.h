#ifndef __PRINTF_H__ 
#define __PRINTF_H__ 

#include <stdarg.h>

int vprintf(const char * format, va_list arg);
int printf(const char * format, ...);
void print_decimal(int number);
void print_unsigned_decimal(unsigned int number);
void print_unsigned_decimal_hex(unsigned int number);
void print_unsigned_decimal_oct(unsigned int number);
void print_char(char number);
void print_signed_char(char number);
void print_unsigned_char(unsigned char number);
void print_short(short number);
void print_unsigned_short(unsigned short number);
void print_long(long number);
void print_unsigned_long(unsigned long number);
void print_long_long(long long number);
void print_unsigned_long_long(unsigned long long number);
void print_unsigned_long_long_hex(unsigned long long number);
void reverse(char * str);

#endif //__PRINTF_H__ 
