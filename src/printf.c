#include <printf.h>
#include <stdarg.h>
#include <iolib.h>
#include <stdint.h>

#define __UNSIGNED \
    char str[50]; \
    if (number == 0) { \
        print_char('0'); \
        return; \
    } \
    int i = 0; \
    while (number != 0) { \
        str[i++] = '0' + number % 10; \
        number /= 10;  \
    } \
    str[i] = 0; \
    reverse(str); \
    write(str); \

#define __SIGNED \
    char str[50]; \
    if (number == 0) { \
        print_char('0'); \
        return; \
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
        print_char('-'); \
    } \
    write(str); \


int vprintf(const char * format, va_list arg)
{
    int err = 0;
    int l = 0;
    while (format[l]) {
        if (format[l] == '%') {
            l++;
            if (!format[l]) {
                return 1;
            }
            switch (format[l])
            {
                case 'p':  {
                    print_unsigned_long_long_hex(va_arg(arg, unsigned long long));
                    break;
                }
                case 'c':  {
                    print_char(va_arg(arg, int));
                    break;
                }
                case 'd':
                case 'i':  {
                    print_decimal(va_arg(arg, int));
                    break;
                }
                case 'o': {
                    print_unsigned_decimal_oct(va_arg(arg, unsigned int));
                    break;
                }
                case 'x': {
                    print_unsigned_decimal_hex(va_arg(arg, uint64_t));
                    break;
                }
                case 'u': {
                    print_unsigned_decimal(va_arg(arg, unsigned int));
                    break;
                }
                case 's':  {
                    write(va_arg(arg, char *));
                    break;
                }
                case 'h': {
                    l++;
                    if (!format[l]) {
                        return 1;
                    } else
                    if (format[l] == 'h' && (format[l + 1] == 'i' || format[l + 1] == 'd')) {
                        print_signed_char(va_arg(arg, int));
                        l++;
                    } else
                    if (format[l] == 'h' && (format[l + 1] == 'u' || format[l + 1] == 'o')) {
                        print_unsigned_char(va_arg(arg, int));
                        l++;
                    } else
                    if (format[l] == 'i' || format[l] == 'd') {
                        print_short(va_arg(arg, int));
                    } else
                    if (format[l] == 'u' || format[l] == 'o' || format[l] == 'x') {
                        print_unsigned_short(va_arg(arg, int));
                    }

                    break;
                }
                case 'l': {
                    l++;
                    if (!format[l]) {
                        return 1;
                    } else
                    if (format[l] == 'l' && (format[l + 1] == 'i' || format[l + 1] == 'd')) {
                        print_long_long(va_arg(arg, long long));
                        l++;
                    } else
                    if (format[l] == 'l' && (format[l + 1] == 'u' || format[l + 1] == 'o' || format[l + 1] == 'x')) {
                        print_unsigned_long_long(va_arg(arg, unsigned long long));
                        l++;
                    } else
                    if (format[l] == 'i' || format[l] == 'd') {
                        print_long(va_arg(arg, long));
                    } else
                    if (format[l] == 'u' || format[l] == 'o' || format[l] == 'x') {
                        print_unsigned_long(va_arg(arg, unsigned long));
                    }
                    break;
                }
                default:
                    err = 2;
            }
        } else {
            print_char(format[l]);
        }
        l++;
    }

    return err;
}

int printf(const char * format, ...)
{
    va_list arg;
    va_start(arg, format);
    int err = vprintf(format, arg);
    va_end(arg);
    return err;
}

void print_decimal(int number)
{
    __SIGNED
}

void print_unsigned_decimal(unsigned int number)
{
    __UNSIGNED
}


void print_unsigned_decimal_hex(uint64_t number)
{
    if (number == 0) {
        print_char('0');
        return;
    }
    char str[50];
    int i = 0;
    while (number != 0) {
        str[i++] = ( number % 16 > 9 ? 'a' + number % 16 - 10 : '0' + number % 16 );
        number /= 16;
    }
    str[i] = 0;
    reverse(str);
    write(str);
}

void print_unsigned_decimal_oct(unsigned int number)
{
    if (number == 0) {
        print_char('0');
        return;
    }
    char str[50];
    int i = 0;
    while (number != 0) {
        str[i++] = '0' + number % 8;
        number /= 8;
    }
    str[i] = 0;
    reverse(str);
    write(str);
}

void print_char(char ch)
{
    char str[2];
    str[0] = ch;
    str[1] = 0;
    write(str);
}

void print_signed_char(char number)
{
    __SIGNED
}

void print_unsigned_char(unsigned char number)
{
    __UNSIGNED
}

void print_short(short number)
{
    __SIGNED
}

void print_unsigned_short(unsigned short number)
{
    __UNSIGNED
}

void print_long(long number)
{
    __SIGNED
}

void print_unsigned_long(unsigned long number)
{
    __UNSIGNED
}

void print_long_long(long long number)
{
    __SIGNED
}

void print_unsigned_long_long(unsigned long long number)
{
    __UNSIGNED
}

void reverse(char * str)
{
    int size = 0;
    while (str[size]) {
        size++;
    }

    for (int i = 0; i < size / 2; i++) {
        int tmp = str[size - i - 1];
        str[size - i - 1] = str[i];
        str[i] = tmp;
    }
}

void print_unsigned_long_long_hex(unsigned long long number)
{
    if (number == 0) {
        print_char('0');
        return;
    }
    char str[50];
    int i = 0;
    while (number != 0) {
        str[i++] = ( number % 16 > 9 ? 'a' + number % 16 - 10 : '0' + number % 16 );
        number /= 16;
    }
    str[i] = 0;
    reverse(str);
    write(str);
}

#undef __UNSIGNED
#undef __SIGNED
