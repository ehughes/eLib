#include "stdint.h"
#include "IO_EXP.h"
#include "MCP23S17.h"

volatile uint8_t TestData = 0;


uint8_t Expander_Misc__A_Shadow = 0;
uint8_t Expander_Misc__B_Shadow = 0;


uint8_t Expander_Misc__A_Dirty;
uint8_t Expander_Misc__B_Dirty;


void IO_EXP_Init()
{
	 MCP23S17_Init();

	 MCP23S17_WriteRegister_8Bit(EXPANDER_CS_CHANNEL_MISC,0,MCP23S17_BANK0_IOCON,MCP23S17_IOCON_BIT_HAEN);


	 /*
	  * Setup Data Direction Reg
	  *
	  */

	 MCP23S17_WriteRegister_8Bit(EXPANDER_CS_CHANNEL_MISC,0,MCP23S17_BANK0_IODIRB,~(ACT_LED_C_PIN));



}
