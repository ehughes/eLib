#include "System.h"
#include "IO_EXP_Config.h"

#define MCP23S17_NUM_CHANNELS   1


void MCP23S17_CS_Active(uint8_t ExpanderChannel)
{
      EXPANDER_CS_LOW;
}

void MCP23S17_CS_Inactive(uint8_t ExpanderChannel)
{
      EXPANDER_CS_HIGH;
}


uint8_t MCP23S17_TxRx(uint8_t DataOut)
{
     return Expander_SPI_RW(DataOut);
}


void MCP23S17_Init()
{
        for(int i=0;i<MCP23S17_NUM_CHANNELS;i++)
        {
                MCP23S17_CS_Inactive(i);
        }


        for(int i=0;i<MCP23S17_NUM_CHANNELS;i++)
        {
                MCP23S17_CS_Active(i);
        }

        System__Delay_mS(10);

        for(int i=0;i<MCP23S17_NUM_CHANNELS;i++)
        {
                MCP23S17_CS_Inactive(i);
        }
}

void MCP23S17_WriteRegister_8Bit(uint8_t CS_Channel, uint8_t HardwareChannel,uint8_t Register, uint8_t Data)
{
        uint8_t ControlByte = 0x40 | ((HardwareChannel&0x7)<<1);

        MCP23S17_CS_Active(CS_Channel);

        (void)MCP23S17_TxRx(ControlByte);
        (void)MCP23S17_TxRx(Register);
        (void)MCP23S17_TxRx(Data);


        MCP23S17_CS_Inactive(CS_Channel);
}

#define CONTROL_READ_BIT (1<<0)

uint8_t MCP23S17_ReadRegister_8Bit(uint8_t CS_Channel, uint8_t HardwareChannel,uint8_t Register)
{
        uint8_t ControlByte = 0x40 | ((HardwareChannel&0x7)<<1) | CONTROL_READ_BIT;
        uint8_t Data;

        MCP23S17_CS_Active(CS_Channel);

        (void)MCP23S17_TxRx(ControlByte);
        (void)MCP23S17_TxRx(Register);
        Data = MCP23S17_TxRx(0x00);


        MCP23S17_CS_Inactive(CS_Channel);

        return Data;
}

