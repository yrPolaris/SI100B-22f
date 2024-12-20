#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int main()
{
    int32_t a = 0x80000000;
    uint32_t b = 0x80000000;
    int32_t c = -1;
    printf("%d\n", (0x80000000 | c) >> 31);
    return 0;
}