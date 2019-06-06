#include "MasterConfiguration.h"
#include "Board.h"


#if (BOARD_HAS_I2C == 1) && (ENABLE_ADT7420 == 1)

#include "ADT7420.h"

#include "System_I2C.h"

#define ADT7420_ADDRESS (0x49)

#define ADT7420_REGISTER_TEMPERATURE            0x00
#define ADT7420_REGISTER_CONFIGURATION          0x03


void ADT7420_Init(uint8_t AD7420_Address)
{
        System_I2C_WriteRegister_8Bit(AD7420_Address,ADT7420_REGISTER_CONFIGURATION,0x80); //Setup for 16-bit mode
}



float ADT7420_GetTemperature(uint8_t AD7420_Address)
{

        static int16_t TempValue=0x00;
        float Temperature = 0;

        System_I2C_ReadRegister_16Bit(AD7420_Address,ADT7420_REGISTER_TEMPERATURE, (uint16_t *)&TempValue);

        //For now we are using 13 bit data so round and then truncate.
        //TempValue += ((1<<3)+1)/2;
//      TempValue=TempValue>>3;


        Temperature = (((TempValue))/128.0f);


        return Temperature;
}

#endif
