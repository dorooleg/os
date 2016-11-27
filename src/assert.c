#include <assert.h>
#include <printf.h>

inline void assert(char condition, const char * msg)
{
    if (!condition) {
        printf("Error assert: %s\n", msg);
    }
}
