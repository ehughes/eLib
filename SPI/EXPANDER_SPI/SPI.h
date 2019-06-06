#include "stdint.h"

#ifndef _SYSTEM_SPI
#define _SYSTEM_SPI

void Expander_SPI_Init();

uint8_t Expander_SPI_RW(uint8_t DataOut);

#endif
