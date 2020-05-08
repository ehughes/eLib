

#include <stdint.h>

#ifndef W25Q_FLASH_H_
#define W25Q_FLASH_H_

void W25Q_WRSR(uint8_t StatusReg);
uint8_t W25Q_RDSR();
void W25Q_RDID(uint8_t *ID);
void W25Q_WREN();
void W25Q_READ(uint32_t SectorAddress, uint8_t *ReadBuffer);
void W25Q_BE();
void W25Q_SE(uint32_t SectorAddress);
void W25Q_4KSE(uint32_t SectorAddress);
void W25Q_PP(uint32_t SectorAddress, uint8_t *WriteBuffer);
void W25Q_RES();
void W25Q_DP();
void W25Q_PE(uint32_t PageAddress);
void W25Q_PW(uint32_t PageAddress, uint8_t *WriteBuffer);


void W25Q_FlashRead(uint32_t StartAddress, uint16_t Length, uint8_t *DataBuffer);
void W25Q_BlockUntilFlashOperationComplete();

#define W25Q_ATTACHED_FLASH_SIZE_IN_BYTES (16777216)


#endif
