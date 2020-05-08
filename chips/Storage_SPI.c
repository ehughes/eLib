
#include <Drivers/Storage/Storage_SPI.h>
#include "System.h"



uint8_t STORAGE_SPI_RW(uint8_t Data)
{
	LPC_SSP1->DR = Data;

	while((LPC_SSP1->SR & (1<<4)) >0){}

	return LPC_SSP1->DR;

}


void Storage_SPI__Init()
{
    //We assume the I/O pins have already been setup
    Chip_Clock_Enable(CLK_MX_SSP1);
    Chip_SSP_Set_Mode(LPC_SSP1, SSP_MODE_MASTER);
    Chip_SSP_SetFormat(LPC_SSP1, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA0_CPOL0);
    Chip_SSP_SetBitRate(LPC_SSP1, 8000000);
    Chip_SSP_Enable(LPC_SSP1);
}
