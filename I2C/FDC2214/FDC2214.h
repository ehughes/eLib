#include "MasterConfiguration.h"
#include "Board.h"

#ifdef BOARD_HAS_I2C

#include "stdint.h"

#ifndef __FDC2214_H
#define __FDC2214_H

void FDC2214_Init();

void FDC2214_GetID(uint16_t *ManfID,uint16_t *DeviceID);

uint32_t FDC2214_CH0();
uint32_t FDC2214_CH1();


#define FDC2214_I2C_ADDRESS (0x2A)

#endif

#endif
