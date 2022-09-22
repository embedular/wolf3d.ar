#include <stdint.h>
#include <stdio.h>


char* itoa(int value, char* string, int radix)
{
    (void) radix;
    sprintf(string, "%d", value);
    return string;
}


char* ltoa(long value, char* string, int radix)
{
    (void) radix;
    sprintf(string, "%ld", value);
    return string;
}


uint16_t READWORD(uint8_t **ptr)
{
    const uint8_t *p = *ptr;
    const uint16_t val = p[0] | p[1] << 8;
    *ptr += 2;
    return val;
}

uint32_t READLONGWORD(uint8_t **ptr)
{
    const uint8_t *p = *ptr;
    const uint32_t val = p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
    *ptr += 4;
    return val;
}
