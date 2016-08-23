#include <stdint.h>

#ifndef FLASH_H_
#define FLASH_H_

void M25PE_WRSR(uint8_t StatusReg);
uint8_t M25PE_RDSR();
void M25PE_RDID(uint8_t *ID);
void M25PE_WREN();
void M25PE_READ(uint32_t SectorAddress, uint8_t *ReadBuffer);
void M25PE_BE();
void M25PE_SE(uint32_t SectorAddress);
void M25PE_PP(uint32_t SectorAddress, uint8_t *WriteBuffer);
void M25PE_RES();
void M25PE_DP();
void M25PE_PE(uint32_t PageAddress);
void M25PE_PW(uint32_t PageAddress, uint8_t *WriteBuffer);

void InitM25P_FLASH();
void FlashRead(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer);
void M25PE_BlockUntilFlashOperationComplete();


#define M25P_ATTACHED_FLASH_SIZE_IN_BYTES (2097152)

#endif

