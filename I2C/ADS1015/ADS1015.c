#include <I2C/ADS1015/ADS1015.h>
#include "Drivers/I2C/System_I2C.h"


#define ADS1015_ADDRESS (0x48)


#define ADS1015_REGISTER_CONVERSION		0x00
#define ADS1015_REGISTER_CONFIG			0x01
#define ADS1015_REGISTER_LO_THRES		0x02
#define ADS1015_REGISTER_HIGH_THRES		0x03


//PGA set for +/- 2.048
//Continuous mode
//1600SPS
#define ADS1015_BASE_CONFIG (2<<9) |\
							(4<<5)


void ADS1015_SelectChannel(uint8_t C)
{
	if(C)
	{
		System_I2C_WriteRegister_16Bit(ADS1015_ADDRESS,ADS1015_REGISTER_CONFIG,ADS1015_BASE_CONFIG | (5<<12) ); //Setup for 16-bit mode
	}
	else
	{
		System_I2C_WriteRegister_16Bit(ADS1015_ADDRESS,ADS1015_REGISTER_CONFIG,ADS1015_BASE_CONFIG | (4<<12)); //Setup for 16-bit mode
	}

}
void ADS1015_Init()
{
	ADS1015_SelectChannel(0);
}


int16_t ADS1015_Read()
{
	int16_t V = 0;

	System_I2C_ReadRegister_16Bit(ADS1015_ADDRESS, ADS1015_REGISTER_CONVERSION, (uint16_t *)&V);

	return V;
}

uint16_t ADS1015_ReadConfig()
{
	uint16_t V = 0;

	System_I2C_ReadRegister_16Bit(ADS1015_ADDRESS, ADS1015_REGISTER_CONFIG, &V);

	return V;

}
