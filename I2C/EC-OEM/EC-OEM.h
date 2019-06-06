#include "MasterConfiguration.h"

#if (BOARD_HAS_I2C == 1) && (ENABLE_CONDUCTIVITY == 1)


#ifndef __EC_OEM_H
#define __EC_OEM_H

#define EC_OEM_ADDRESS (0x64)


#define EC_OEM_CAL_TYPE__CLEAR     0x01
#define EC_OEM_CAL_TYPE__DRY       0x02
#define EC_OEM_CAL_TYPE__SINGLE    0x03
#define EC_OEM_CAL_TYPE__DUAL_LOW  0x04
#define EC_OEM_CAL_TYPE__DUAL_HIGH 0x05


typedef  struct
{
    uint8_t DeviceType;
    uint8_t FirmwareVersion;
   
    union
    {
       uint16_t U_Int16;
       uint8_t Bytes[2];
    } ProbeType;
   
   float K;

   uint8_t CalibrationConfirmation;

    union
    {
       uint32_t U_Int32;
       uint8_t Bytes[4];
    } LastReading;
   
    float TemperatureConfirmation;

} __attribute__((packed)) EC_OEM_DeviceInfo;

void   EC_OEM_GetDeviceInfo(EC_OEM_DeviceInfo * EC);
void   EC_StartStop(uint8_t Start);
void   EC_SendCalibration(float CalValue,uint32_t CAL_Type);
uint32_t EC_OEM_GetRawReading();
float EC_OEM_GetTempConfiguration();
void  EC_SendTemperature(float Temperature);

#endif

#endif

