#include <stdint.h>

#ifndef FRAM_H_
#define FRAM_H_

void InitFRAM();
//A high level function to read up to 65k bytes from the Flash device
void  FRAM_Read(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer);

//A high level function to Write up to 65k bytes from the Flash device
void  FRAM_Write(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer);

void FRAM_WREN();

//This function acquires the identification from the M25PX device (Blocking SPI)
void FRAM_RDID(uint8_t *ID);

uint8_t FRAM_RDSR();

void FRAM_WRSR(uint8_t StatusReg);

//A high level function to Write up to 65k bytes of a single value to an area of the FRAM
void  FRAM_WriteConst(uint32_t StartAddress, uint16_t Length, uint8_t Data);


#define FRAM_ATTACHED_FLASH_SIZE_IN_BYTES (1<<17)

#endif

