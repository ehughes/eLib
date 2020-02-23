#include "stdint.h"
#include "System/Drivers/I2C/System_I2C.h"
#include "ADT7420.h"

#define ADT7420_ADDRESS (0x49)

#define ADT7420_REGISTER_TEMPERATURE		0x00
#define ADT7420_REGISTER_CONFIGURATION		0x03


void ADT7420_Init(uint8_t AD7420_Address)
{
	System_I2C_WriteRegister_8Bit(AD7420_Address,ADT7420_REGISTER_CONFIGURATION,0x80); //Setup for 16-bit mode
}



bool ADT7420_GetTemperature(uint8_t AD7420_Address,float *Temperature)
{

	static int16_t TempValue=0x00;

	if(System_I2C_ReadRegister_16Bit(AD7420_Address,ADT7420_REGISTER_TEMPERATURE, (uint16_t *)&TempValue) != kStatus_Success)
	{
		*Temperature = (((TempValue))/128.0f);
		return false;
	}

	if(Temperature!=NULL)
	{
		*Temperature = (((TempValue))/128.0f);
		return true;
	}

	return false;
}
