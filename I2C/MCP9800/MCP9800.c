#include "System/Drivers/I2C/System_I2C.h"
#include "MCP9800.h"




#define MCP9800_REGISTER_TEMPERATURE		0x00
#define MCP9800_REGISTER_CONFIGURATION		0x01
#define MCP9800_REGISTER_HYSTERESIS			0x02
#define MCP9800_REGISTER_LIMIT_SET			0x03


void MCP9800_Init(uint8_t MCP9800_Address)
{
	//12-bit mode
	System_I2C_WriteRegister_8Bit(MCP9800_Address,MCP9800_REGISTER_CONFIGURATION, 0x3<<5);
}

float MCP9800_GetTemperature(uint8_t MCP9800_Address)
{
	uint16_t Value=0x00;

	System_I2C_ReadRegister_16Bit(MCP9800_Address,MCP9800_REGISTER_TEMPERATURE, &Value);

	return ((float)(Value)/256.0f);
}
