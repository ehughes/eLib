#include "MasterConfiguration.h"


#include <stdint.h>
#include "nrfx.h"

#ifndef __SYSTEM_I2C_H
#define __SYSTEM_I2C_H


typedef enum
{
    I2C_OK = 0,          
    I2C_INIT_ERROR = 1,
    I2C_TX_ERROR  = 2,
    I2C_RX_ERROR  =  3,
    I2C_TXRX_ERROR  = 4,
    I2C_NACK = 5              
} I2C_Result;


I2C_Result System_I2C_Init();

I2C_Result System_I2C_WriteRaw(uint8_t DeviceI2C_Address, uint8_t *RawData, uint8_t Length);

I2C_Result System_I2C_ReadRaw(uint8_t DeviceI2C_Address, uint8_t *RawData, uint8_t Length);


I2C_Result System_I2C_TxRxRaw(uint8_t DeviceI2C_Address, 
                                   uint8_t *TX_RawData, 
                                   uint8_t TX_Length,
                                   uint8_t *RX_RawData, 
                                   uint8_t RX_Length
                                   );

I2C_Result System_I2C_WriteRegister_8Bit(uint8_t DeviceI2C_Address,uint8_t Register, uint8_t Value_8Bit);

I2C_Result System_I2C_WriteRegister_16Bit(uint8_t DeviceI2C_Address, uint8_t Register, uint16_t Value_16Bit);

I2C_Result System_I2C_ReadRegister_8Bit(uint8_t DeviceI2C_Address,uint8_t Register, uint8_t *Value_8Bit);

I2C_Result System_I2C_ReadRegister_16Bit(uint8_t DeviceI2C_Address,uint8_t Register, uint16_t *Value_16Bit);



//#define SYSTEM_I2C_RTOSIFY

#endif

