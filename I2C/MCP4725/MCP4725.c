
#include "System/Drivers/I2C/System_I2C.h"
#include "MCP4725.h"


#define MCP4725_ADDRESS	    (0x61)
#define MCP4725_I2C     	I2C3



status_t MCP4725_WriteDAC_Fast(uint16_t Value_12Bit)
{

    uint8_t Buffer[2];

    Buffer[0] = (Value_12Bit>>8) & 0xF;
    Buffer[1] = (Value_12Bit&0xFF);

    return System_I2C_WriteRaw(MCP4725_ADDRESS,Buffer,2);

}


void MCP4725_Init()
{
	MCP4725_WriteDAC_Fast(0);
}

