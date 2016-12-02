#include <lock.h>
#include <printf.h>
#include <ints.h>

static int icnt;
void locki()
{
    disable_ints();
    icnt++;
}

void unlocki()
{
    icnt--;
    if (icnt == 0)
        enable_ints();
}
