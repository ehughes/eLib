#include <stdint.h>

#ifndef FRAM_H_
#define FRAM_H_

//A high level function to read up to 65k bytes from the Flash device
void  FRAM_Read(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer);

//A high level function to Write up to 65k bytes from the Flash device
void  FRAM_Write(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer);

void FRAM_WREN();

uint8_t FRAM_RDSR();

void FRAM_WRSR(uint8_t StatusReg);

#define FRAM_ATTACHED_FLASH_SIZE_IN_BYTES (1<<13)


#endif

