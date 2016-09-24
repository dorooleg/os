#ifndef __SNPRINTF_H__ 
#define __SNPRINTF_H__ 

#include <stdarg.h>
#include <stdint.h>

int snprintf(char * buf, uint64_t n, const char * format, ...);
int vsnprintf(char * buf, uint64_t n, const char * format, va_list arg);
int snprint_decimal(char * buf, uint64_t pos, uint64_t n, int number);
int snprint_unsigned_decimal(char * buf, uint64_t pos, uint64_t n, unsigned int number);
int snprint_unsigned_decimal_hex(char * buf, uint64_t pos, uint64_t n, unsigned int number);
int snprint_unsigned_decimal_oct(char * buf, uint64_t pos, uint64_t n, unsigned int number);
int snprint_char(char * buf, uint64_t pos, uint64_t n, char number);
int snprint_signed_char(char * buf, uint64_t pos, uint64_t n, char number);
int snprint_unsigned_char(char * buf, uint64_t pos, uint64_t n, unsigned char number);
int snprint_short(char * buf, uint64_t pos, uint64_t n, short number);
int snprint_unsigned_short(char * buf, uint64_t pos, uint64_t n, unsigned short number);
int snprint_long(char * buf, uint64_t pos, uint64_t n, long number);
int snprint_unsigned_long(char * buf, uint64_t pos, uint64_t n, unsigned long number);
int snprint_long_long(char * buf, uint64_t pos, uint64_t n, long long number);
int snprint_unsigned_long_long(char * buf, uint64_t pos, uint64_t n, unsigned long long number);
int snprint_string(char * buf, uint64_t pos, uint64_t n, char * str);


#endif //__SNPRINTF_H__ 
