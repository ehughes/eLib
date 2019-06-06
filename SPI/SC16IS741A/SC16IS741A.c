#include "stdint.h"
#include "MasterConfiguration.h"
#include "Board.h"
#include "SC16IS741A.h"
#include "System.h"

#define  SC16IS741A_XTAL_FREQ 3686400



#define SC16IS741A_SPI                NRF_SPI2


void SC16IS741A_SPI_Init()
{
    SC16IS741A_SPI->ENABLE = 0;
    SC16IS741A_SPI->PSEL.SCK = NRF_GPIO_PIN_MAP(SC16IS741A_SPI_CLOCK_PORT_NUMBER,SC16IS741A_SPI_CLOCK_PIN);
        
    SC16IS741A_SPI->PSEL.MOSI = NRF_GPIO_PIN_MAP(SC16IS741A_SPI_MOSI_PORT_NUMBER,SC16IS741A_SPI_MOSI_PIN );
    SC16IS741A_SPI->PSEL.MISO = NRF_GPIO_PIN_MAP(SC16IS741A_SPI_MISO_PORT_NUMBER,SC16IS741A_SPI_MISO_PIN );
        
    SC16IS741A_SPI->FREQUENCY = SPI_FREQUENCY_FREQUENCY_M4;
    SC16IS741A_SPI->CONFIG = 0;
           
    SC16IS741A_SPI->ENABLE = 1;
}


uint8_t SC16IS741A_SPI_RW(uint8_t DataOut)
{
    uint8_t Junk;
    Junk = SC16IS741A_SPI->RXD;
    SC16IS741A_SPI->EVENTS_READY = 0;
    SC16IS741A_SPI->TXD = DataOut;

    while(SC16IS741A_SPI->EVENTS_READY == 0)
    {
    }

    SC16IS741A_SPI->EVENTS_READY = 0;
    return SC16IS741A_SPI->RXD;
}



void SC16IS741A_WriteRegister(uint8_t Register, uint8_t Value)
{
    SC16IS741A_CS_LOW;

   (void)SC16IS741A_SPI_RW((Register<<3));
   (void)SC16IS741A_SPI_RW(Value);

    SC16IS741A_CS_HIGH;
}

void SC16IS741A_WriteTxFIFO(uint8_t *Value, uint32_t Length)
{
    SC16IS741A_CS_LOW;

   (void)SC16IS741A_SPI_RW(0);
   
   for(int i=0;i<Length;i++)
   {
        (void)SC16IS741A_SPI_RW(Value[i]);
   }
   
    SC16IS741A_CS_HIGH;
}


void SC16IS741A_ReadRxFIFO(uint8_t *Value, uint32_t Length)
{
    SC16IS741A_CS_LOW;

   (void)SC16IS741A_SPI_RW(SC16IS741A__READ_REG);
   
   for(int i=0;i<Length;i++)
   {
        Value[i]  =  SC16IS741A_SPI_RW(0);
   }
   
    SC16IS741A_CS_HIGH;
}


uint8_t SC16IS741A_ReadRegister(uint8_t Register)
{
    uint8_t Ret;

    SC16IS741A_CS_LOW;

    (void)SC16IS741A_SPI_RW(SC16IS741A__READ_REG | (Register<<3));
    Ret = SC16IS741A_SPI_RW(0);
    
    SC16IS741A_CS_HIGH;

    return Ret;
}



void SC16IS741A_Init()
{

    SC16IS741A_SPI_Init();

    uint8_t Output = 0xFF;

    //Set the DLB to be able to hit the baud registers
    SC16IS741A_WriteRegister(SC16IS741A__LCR, SC16IS741A__LCR_BIT__DIVISOR_LATCH_ENABLE);

    SC16IS741A_WriteRegister(SC16IS741A__SPECIAL_DLL, 2);
    SC16IS741A_WriteRegister(SC16IS741A__SPECIAL_DLH, 0);

    //Turn off the DLB and configure for no partiy 1 stop bit and 8 bit word
    SC16IS741A_WriteRegister(SC16IS741A__LCR, SC16IS741A__LCR_BITS__WORD_LENGTH_8BIT);

    SC16IS741A_WriteRegister(SC16IS741A__FCR, SC16IS741A__FCR_BITS__FIFO_ENABLE);

  
}

