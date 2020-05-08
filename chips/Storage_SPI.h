#include "stdint.h"
#ifndef STORAGE_SPI_H
#define STORAGE_SPI_H

#define STORAGE_SPI_WRITE_REG		LPC_SSP1->DR
#define STORAGE_SPI_READ_REG  		LPC_SSP1->DR
#define STORAGE_SPI_WAIT_FOR_SPI	while((LPC_SSP1->SR & (1<<4))>0){}

uint8_t STORAGE_SPI_RW(uint8_t Data);

void Storage_SPI__Init();

#endif
