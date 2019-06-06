
#include "stdint.h"
#include "BCD.h"

uint8_t   CONVERT_0_99_INT_TO_BCD(uint8_t Input)

{
    uint8_t BCD;

    if(Input > 99)
        Input == 99;

    return ((Input / 10) << 4) | (Input %10);
}

uint8_t   CONVERT_BCD_TO_0_99_INT(uint8_t Input)
{
    if(Input > 0x99)
        Input == 0x99;

    return (((Input & 0xF0) >> 4) * 10) + (Input & 0x0F);
}
