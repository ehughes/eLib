#include "MasterConfiguration.h"
#include "Board.h"

#if (BOARD_HAS_I2C == 1) && (ENABLE_CONDUCTIVITY == 1)

#include "System_I2C.h"
#include "EC-OEM.h"

#define EC_OEM_REGISTER__DEVICE_TYPE              0x00
#define EC_OEM_REGISTER__DEVICE_FIRMWARE_VERSION  0x01
#define EC_OEM_REGISTER__DEVICE_ACTIVATE          0x06

#define EC_OEM_REGISTER__PROBE_TYPE_MSB           0x08
#define EC_OEM_REGISTER__PROBE_TYPE_LSB           0x09

#define EC_OEM_REGISTER__CAL_MSB                  0x0A
#define EC_OEM_REGISTER__CAL_HIGH                 0x0B
#define EC_OEM_REGISTER__CAL_LOW                  0x0C
#define EC_OEM_REGISTER__CAL_LSB                  0x0D

#define EC_OEM_REGISTER__CAL_REQUEST              0x0E
#define EC_OEM_REGISTER__CAL_CONFIRMATION         0x0F

#define EC_OEM_REGISTER__TEMP_COMP_MSB            0x10
#define EC_OEM_REGISTER__TEMP_COMP_HIGH           0x11
#define EC_OEM_REGISTER__TEMP_COMP_LOW            0x12
#define EC_OEM_REGISTER__TEMP_COMP_LSB            0x13

#define EC_OEM_REGISTER__TEMP_CONF_MSB            0x14
#define EC_OEM_REGISTER__TEMP_CONF_HIGH           0x15
#define EC_OEM_REGISTER__TEMP_CONF_LOW            0x16
#define EC_OEM_REGISTER__TEMP_CONF_LSB            0x17

#define EC_OEM_REGISTER__READING_MSB              0x18
#define EC_OEM_REGISTER__READING_HIGH             0x19
#define EC_OEM_REGISTER__READING_LOW              0x1A
#define EC_OEM_REGISTER__READING_LSB              0x1B


uint32_t EC_OEM_GetRawReading()
{
    uint8_t DataOut;

    union
    {
       uint32_t U_Int32;
       uint8_t Bytes[4];
    } LastReading;

    DataOut = EC_OEM_REGISTER__READING_MSB;
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
    System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&LastReading.Bytes[3],1);
    DataOut = EC_OEM_REGISTER__READING_HIGH;
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
    System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&LastReading.Bytes[2],1);
    DataOut = EC_OEM_REGISTER__READING_LOW;
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
    System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&LastReading.Bytes[1],1);
    DataOut = EC_OEM_REGISTER__READING_LSB;
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
    System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&LastReading.Bytes[0],1);

    return LastReading.U_Int32;
}

void   EC_OEM_GetDeviceInfo(EC_OEM_DeviceInfo * EC)
{
     uint8_t DataOut = EC_OEM_REGISTER__DEVICE_TYPE;
     System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
     System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)EC,2);

     DataOut = EC_OEM_REGISTER__PROBE_TYPE_MSB;
     System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
     System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&EC->ProbeType.Bytes[1],1);

     DataOut = EC_OEM_REGISTER__PROBE_TYPE_LSB;
     System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
     System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&EC->ProbeType.Bytes[0],1);

     DataOut = EC_OEM_REGISTER__CAL_CONFIRMATION;
     System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
     System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&EC->CalibrationConfirmation,1);

     EC->LastReading.U_Int32 = EC_OEM_GetRawReading();

     EC->TemperatureConfirmation = EC_OEM_GetTempConfiguration();
}

void   EC_StartStop(uint8_t Start)
{
    uint8_t DataOut[2];
    
    DataOut[0] = EC_OEM_REGISTER__DEVICE_ACTIVATE;

    if(Start)
      DataOut[1] = 0x1;
    else
      DataOut[1] = 0x0;

    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut[0], sizeof(DataOut));
}   

void  EC_SendCalibration(float CalValue,uint32_t CAL_Type)
{

   uint8_t DataOut[5];
     
   union
   {
        uint32_t CalValue_U_Int32;
        uint8_t Bytes[4];
   } MyCalValue;

    MyCalValue.CalValue_U_Int32 = (uint32_t)((float)CalValue * 100.0f);

  switch(CAL_Type)
  {
    default:
        return;
    break;

    case EC_OEM_CAL_TYPE__CLEAR:
    case EC_OEM_CAL_TYPE__DRY:
    
    break;

    case EC_OEM_CAL_TYPE__SINGLE:
    case EC_OEM_CAL_TYPE__DUAL_LOW:
    case EC_OEM_CAL_TYPE__DUAL_HIGH:
    
        DataOut[0] = EC_OEM_REGISTER__CAL_MSB;
        DataOut[1] = MyCalValue.Bytes[3];
        DataOut[2] = MyCalValue.Bytes[2];
        DataOut[3] = MyCalValue.Bytes[1];
        DataOut[4] = MyCalValue.Bytes[0];
    
        System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut[0], 5);
       
    break;
  }
   
  DataOut[0] = EC_OEM_REGISTER__CAL_REQUEST;
  DataOut[1] = CAL_Type;

  System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut[0], 2);
}

void  EC_SendTemperature(float Temperature)
{

   uint8_t DataOut[5];
     
   union
   {
        uint32_t U_Int32;
        uint8_t Bytes[4];
   } MyTempValue;

    MyTempValue.U_Int32 = (uint32_t)((float)Temperature * 100.0f);
   
    DataOut[0] = EC_OEM_REGISTER__TEMP_COMP_MSB;
    DataOut[1] = MyTempValue.Bytes[3];
    DataOut[2] = MyTempValue.Bytes[2];
    DataOut[3] = MyTempValue.Bytes[1];
    DataOut[4] = MyTempValue.Bytes[0];
   
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut[0], 5);
      
 }

float EC_OEM_GetTempConfiguration()
{
    uint8_t DataOut;

    union
    {
       uint32_t U_Int32;
       uint8_t Bytes[4];
    } LastReading;

    DataOut = EC_OEM_REGISTER__TEMP_CONF_MSB;
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
    System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&LastReading.Bytes[3],1);
    DataOut = EC_OEM_REGISTER__TEMP_CONF_HIGH;
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
    System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&LastReading.Bytes[2],1);
    DataOut = EC_OEM_REGISTER__TEMP_CONF_LOW;
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
    System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&LastReading.Bytes[1],1);
    DataOut = EC_OEM_REGISTER__TEMP_CONF_LSB;
    System_I2C_WriteRaw(EC_OEM_ADDRESS, &DataOut, 1);
    System_I2C_ReadRaw(EC_OEM_ADDRESS,(uint8_t *)&LastReading.Bytes[0],1);

    return (float)(LastReading.U_Int32)/100.f;
}

#endif
