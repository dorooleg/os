#include <snprintf.h>
#include <printf.h>

#define __UNSIGNED \
    if ((int)pos >= (int)n - 1) { \
        return pos; \
    } \
    char str[50]; \
    if (number == 0) { \
        buf[pos++] = '0'; \
        return pos; \
    } \
    int i = 0; \
    while (number != 0) { \
        str[i++] = '0' + number % 10; \
        number /= 10;  \
    } \
    str[i] = 0; \
    reverse(str); \
    i = 0; \
    while (pos < n - 1 && str[i]) { \
        buf[pos++] = str[i++]; \
    } \
    return pos; \

#define __SIGNED \
    if ((int)pos >= (int)n - 1) { \
        return pos; \
    } \
    char str[50]; \
    if (number == 0) { \
        buf[pos++] = '0'; \
        return pos; \
    } \
    int sign = 0; \
    if (number < 0) { \
        sign = 1; \
        number = -1 * number; \
    } \
    int i = 0; \
    while (number != 0) { \
        str[i++] = '0' + number % 10; \
        number /= 10; \
    } \
    str[i] = 0; \
    reverse(str); \
    if (sign) { \
        buf[pos++] = '-'; \
    } \
    i = 0; \
    while (pos < n - 1 && str[i]) { \
        buf[pos++] = str[i++]; \
    } \
    return pos; \

int snprintf(char * buf, uint64_t n, const char * format, ...)
{
    va_list arg; 
    va_start(arg, format); 
    int count = vsnprintf(buf, n, format, arg);
    va_end(arg);
    return count;
}

int vsnprintf(char * buf, uint64_t n, const char * format, va_list arg)
{
    if (n == 0) {
        return -1;
    }

    if (n == 1) {
        buf[n] = 0;
        return 0;
    }

    int pos = 0;
    int l = 0;
    while (format[l]) {
        if (format[l] == '%') {
            l++;
            if (!format[l]) {
                buf[pos++] = 0;
                return pos;
            }
            switch (format[l])
            {
                case 'c':  {
                    pos = snprint_char(buf, pos, n, va_arg(arg, int));  
                    if (pos >= (int)n - 1) {
                        buf[pos++] = 0;
                        return pos;
                    }
                    break;
                }
                case 'd': 
                case 'i':  {
                    pos = snprint_decimal(buf, pos, n, va_arg(arg, int));  
                    if (pos >= (int)n - 1) {
                        buf[pos++] = 0;
                        return pos;
                    }
                    break;
                }
                case 'o': {
                    pos = snprint_unsigned_decimal_oct(buf, pos, n, va_arg(arg, unsigned int));  
                    if (pos >= (int)n - 1) {
                        buf[pos++] = 0;
                        return pos;
                    }
                    break;
                }
                case 'x': {
                    pos = snprint_unsigned_decimal_hex(buf, pos, n, va_arg(arg, unsigned int));  
                    if (pos >= (int)n - 1) {
                        buf[pos++] = 0;
                        return pos;
                    }
                    break;
                }
                case 'u': {
                    pos = snprint_unsigned_decimal(buf, pos, n, va_arg(arg, unsigned int));  
                    if (pos >= (int)n - 1) {
                        buf[pos++] = 0;
                        return pos;
                    }
                    break;
                }
                case 's':  {
                    pos = snprint_string(buf, pos, n, va_arg(arg, char *));  
                    if (pos >= (int)n - 1) {
                        buf[pos++] = 0;
                        return pos;
                    }
                    break;
                }
                case 'h': {
                    l++;
                    if (!format[l]) {
                        buf[pos++] = 0;
                        return pos;
                    } else
                    if (format[l] == 'h' && (format[l + 1] == 'i' || format[l + 1] == 'd')) {
                        pos = snprint_signed_char(buf, pos, n, va_arg(arg, int));
                        if (pos >= (int)n - 1) {
                            buf[pos++] = 0;
                            return pos;
                        }
                        l++;
                    } else
                    if (format[l] == 'h' && (format[l + 1] == 'u' || format[l + 1] == 'o')) {
                        pos = snprint_unsigned_char(buf, pos, n, va_arg(arg, int));
                        if (pos >= (int)n - 1) {
                            buf[pos++] = 0;
                            return pos;
                        }
                        l++;
                    } else
                    if (format[l] == 'i' || format[l] == 'd') {
                        pos = snprint_short(buf, pos, n, va_arg(arg, int));
                        if (pos >= (int)n - 1) {
                            buf[pos++] = 0;
                            return pos;
                        }
                    } else
                    if (format[l] == 'u' || format[l] == 'o' || format[l] == 'x') {
                        pos = snprint_unsigned_short(buf, pos, n, va_arg(arg, int));
                        if (pos >= (int)n - 1) {
                            buf[pos++] = 0;
                            return pos;
                        }
                    }
                    
                    break;
                }
                case 'l': {
                    l++;
                    if (!format[l]) {
                        buf[pos++] = 0;
                        return pos;
                    } else
                    if (format[l] == 'l' && (format[l + 1] == 'i' || format[l + 1] == 'd')) {
                        pos = snprint_long_long(buf, pos, n, va_arg(arg, long long));
                        if (pos >= (int)n - 1) {
                            buf[pos++] = 0;
                            return pos;
                        }
                        l++;
                    } else
                    if (format[l] == 'l' && (format[l + 1] == 'u' || format[l + 1] == 'o' || format[l + 1] == 'x')) {
                        pos = snprint_unsigned_long_long(buf, pos, n, va_arg(arg, unsigned long long));
                        if (pos >= (int)n - 1) {
                            buf[pos++] = 0;
                            return pos;
                        }
                        l++;
                    } else
                    if (format[l] == 'i' || format[l] == 'd') {
                        pos = snprint_long(buf, pos, n, va_arg(arg, long));
                        if (pos >= (int)n - 1) {
                            buf[pos++] = 0;
                            return pos;
                        }
                    } else
                    if (format[l] == 'u' || format[l] == 'o' || format[l] == 'x') {
                        pos = snprint_unsigned_long(buf, pos, n, va_arg(arg, unsigned long));
                        if (pos >= (int)n - 1) {
                            buf[pos++] = 0;
                            return pos;
                        }
                    }
                }
            }
        } else {
            pos = snprint_char(buf, pos, n, format[l]);
            if (pos >= (int)n - 1) {
                buf[pos++] = 0;
                return pos;
            }
        }
        l++;
    }

    buf[pos++] = 0;
    return pos;
}

int snprint_decimal(char * buf, uint64_t pos, uint64_t n, int number)
{
    __SIGNED
}

int snprint_unsigned_decimal(char * buf, uint64_t pos, uint64_t n, unsigned int number)
{
    __UNSIGNED
}


int snprint_unsigned_decimal_hex(char * buf, uint64_t pos, uint64_t n, unsigned int number)
{
    if ((int)pos >= (int)n - 1) {
        return pos;
    }
    char str[50]; 
    if (number == 0) {
        buf[pos++] = '0';
        return pos;
    }
    int i = 0; 
    while (number != 0) { 
        str[i++] = ( number % 16 > 9 ? 'a' + number % 16 - 10 : '0' + number % 16 ); 
        number /= 16;  
    } 
    str[i] = 0; 
    reverse(str); 

    i = 0;
    while (pos < n - 1 && str[i]) {
        buf[pos++] = str[i++];
    }
    return pos;
}

int snprint_unsigned_decimal_oct(char * buf, uint64_t pos, uint64_t n, unsigned int number)
{
    if ((int)pos >= (int)n - 1) {
        return pos;
    }
    if (number == 0) {
        buf[pos++] = '0';
        return pos;
    }
    char str[50]; 
    int i = 0; 
    while (number != 0) { 
        str[i++] = '0' + number % 8;
        number /= 8;  
    } 
    str[i] = 0; 
    reverse(str); 
    i = 0;
    while (pos < n - 1 && str[i]) {
        buf[pos++] = str[i++];
    }
    return pos;
}

int snprint_char(char * buf, uint64_t pos, uint64_t n, char ch)
{
    if ((int)pos >= (int)n - 1) {
        return pos;
    }
    buf[pos++] = ch;
    return pos;
}

int snprint_signed_char(char * buf, uint64_t pos, uint64_t n, char number) 
{
    __SIGNED
}

int snprint_unsigned_char(char * buf, uint64_t pos, uint64_t n, unsigned char number)
{
    __UNSIGNED
}

int snprint_short(char * buf, uint64_t pos, uint64_t n, short number)
{
    __SIGNED
}

int snprint_unsigned_short(char * buf, uint64_t pos, uint64_t n, unsigned short number)
{
    __UNSIGNED
}

int snprint_long(char * buf, uint64_t pos, uint64_t n, long number)
{
    __SIGNED
}

int snprint_unsigned_long(char * buf, uint64_t pos, uint64_t n, unsigned long number)
{
    __UNSIGNED
}

int snprint_long_long(char * buf, uint64_t pos, uint64_t n, long long number)
{
    __SIGNED
}

int snprint_unsigned_long_long(char * buf, uint64_t pos, uint64_t n, unsigned long long number)
{
    __UNSIGNED
}

int snprint_string(char * buf, uint64_t pos, uint64_t n, char * str)
{
    if ((int)pos >= (int)n - 1) {
        return pos;
    }
    int i = 0;
    while (pos < n - 1 && str[i]) {
        buf[pos++] = str[i++];
    }
    return pos;
}

#undef __UNSIGNED
#undef __SIGNED
