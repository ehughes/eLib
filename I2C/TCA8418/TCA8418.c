#include "../System_I2C.h"
#include "TCA8418.h"

#define TCA8418_ADDRESS 	0x34

void TCA8418_Init()
{

	uint8_t Value;

	//setup the rows
	System_I2C_WriteRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_CFG, 0);

	//setup the rows
	System_I2C_WriteRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_KP_GPIO1, ROW0 | ROW1 | ROW2 | ROW3 | ROW4 | ROW5 | ROW6);

	System_I2C_ReadRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_KP_GPIO1, &Value);

	//setup the columns
	System_I2C_WriteRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_KP_GPIO2, COL0  |COL1| COL2);

	System_I2C_ReadRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_KP_GPIO2, &Value);

	//setup the columns
	System_I2C_WriteRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_KP_GPIO3, 0);

	//Disable key unlock
	System_I2C_WriteRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_UNLOCK1, 0);

	//Disable key unlock
	System_I2C_WriteRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_UNLOCK2, 0);


}


uint8_t TCA8418_GetKeyEventCount()
{
	uint8_t Val =0;

	System_I2C_ReadRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_KEY_LCK_EC, &Val);

	return Val;
}

uint8_t TCA8418_ReadKeyEvent()
{
	uint8_t Val =0;

	System_I2C_ReadRegister_8Bit(TCA8418_ADDRESS, TCA8418_REG_KEY_EVENT_A, &Val);

	return Val;
}
