#include "MasterConfiguration.h"
#include "Board.h"

#if (BOARD_HAS_I2C == 1) && (ENABLE_FDC2214 == 1)

#include "FDC2214.h"
#include "System_I2C.h"



#define FDC2214_DATA_CH0                        0x00
#define FDC2214_DATA_LSB_CH0            0x01
#define FDC2214_DATA_CH1                        0x02
#define FDC2214_DATA_LSB_CH1            0x03
#define FDC2214_DATA_CH2                        0x04
#define FDC2214_DATA_LSB_CH2            0x05
#define FDC2214_DATA_CH3                        0x06
#define FDC2214_DATA_LSB_CH3            0x07
#define FDC2214_RCOUNT_CH0                      0x08
#define FDC2214_RCOUNT_CH1                      0x09
#define FDC2214_RCOUNT_CH2                      0x0A
#define FDC2214_RCOUNT_CH3                      0x0B
#define FDC2214_OFFSET_CH0                      0x0C
#define FDC2214_OFFSET_CH1                      0x0D
#define FDC2214_OFFSET_CH2                      0x0E
#define FDC2214_OFFSET_CH3                      0x0F
#define FDC2214_SETTLECOUNT_CH0         0x10
#define FDC2214_SETTLECOUNT_CH1         0x11
#define FDC2214_SETTLECOUNT_CH2         0x12
#define FDC2214_SETTLECOUNT_CH3         0x13
#define FDC2214_CLOCK_DIVIDERS_CH0      0x14
#define FDC2214_CLOCK_DIVIDERS_CH1      0x15
#define FDC2214_CLOCK_DIVIDERS_CH2      0x16
#define FDC2214_CLOCK_DIVIDERS_CH3      0x17
#define FDC2214_STATUS                          0x18
#define FDC2214_STATUS_CONFIG           0x19
#define FDC2214_CONFIG                  0x1A
#define FDC2214_MUX_CONFIG              0x1B
#define FDC2214_RESET_DEV               0x1C
#define FDC2214_DRIVE_CURRENT_CH0       0x1E
#define FDC2214_DRIVE_CURRENT_CH1       0x1F
#define FDC2214_DRIVE_CURRENT_CH2       0x20
#define FDC2214_DRIVE_CURRENT_CH3       0x21
#define FDC2214_MANUFACTURER_ID         0x7E
#define FDC2214_DEVICE_ID                       0x7F

void  FDC2214_Init()
{

    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_RCOUNT_CH0,         0xFFFF);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_RCOUNT_CH1,         0xFFFF);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_RCOUNT_CH2,         0xFFFF);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_RCOUNT_CH3,         0xFFFF);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_OFFSET_CH0,         0x0000);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_OFFSET_CH1,         0x0000);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_OFFSET_CH2,         0x0000);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_OFFSET_CH3,         0x0000);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_SETTLECOUNT_CH0,    0xFFFF);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_SETTLECOUNT_CH1,    0x0040);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_SETTLECOUNT_CH2,    0x0040);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_SETTLECOUNT_CH3,    0x0040);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_CLOCK_DIVIDERS_CH0, 0x1001);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_CLOCK_DIVIDERS_CH1, 0x1001);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_CLOCK_DIVIDERS_CH2, 0x1001);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_CLOCK_DIVIDERS_CH3, 0x1001);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_STATUS_CONFIG,       0x0001);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_CONFIG,             0x1E01 | (1<<6));
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_MUX_CONFIG,         0x8209 );
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_DRIVE_CURRENT_CH0,  0xFC40);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_DRIVE_CURRENT_CH1,  0xFC40);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_DRIVE_CURRENT_CH2,  0xFC40);
    System_I2C_WriteRegister_16Bit(FDC2214_I2C_ADDRESS, FDC2214_DRIVE_CURRENT_CH3,  0xFC40);

}

void FDC2214_GetID(uint16_t *ManfID,uint16_t *DeviceID)
{

    System_I2C_ReadRegister_16Bit(FDC2214_I2C_ADDRESS,FDC2214_MANUFACTURER_ID,ManfID);
    System_I2C_ReadRegister_16Bit(FDC2214_I2C_ADDRESS,FDC2214_DEVICE_ID,DeviceID);

}

uint32_t FDC2214_CH0()
{

  uint16_t Value;
  uint32_t Result;

  System_I2C_ReadRegister_16Bit(FDC2214_I2C_ADDRESS,FDC2214_DATA_CH0,&Value);
  
  Result = 0;

  Result = ((uint32_t)Value&0xFFF)<<16;

  System_I2C_ReadRegister_16Bit(FDC2214_I2C_ADDRESS,FDC2214_DATA_LSB_CH0,&Value);
  
  Result |= (Value&0xFFFF);

  return Result;

}


uint32_t FDC2214_CH1()
{

  uint16_t Value;
  uint32_t Result;

  System_I2C_ReadRegister_16Bit(FDC2214_I2C_ADDRESS,FDC2214_DATA_CH1,&Value);
  
  Result = 0;

  Result = ((uint32_t)Value&0xFFF)<16;

  System_I2C_ReadRegister_16Bit(FDC2214_I2C_ADDRESS,FDC2214_DATA_LSB_CH1,&Value);
  
  Result |= (Value&0xFFFF);

  return Result;

}
#endif
