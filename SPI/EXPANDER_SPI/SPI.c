#include "stdint.h"
#include "MasterConfiguration.h"
#include "Board.h"
#include "nrf_gpio.h"
#include "nrf.h"


#define EXPANDER_SPI                NRF_SPI1


void Expander_SPI_Init()
{
    EXPANDER_SPI->ENABLE = 0;
    EXPANDER_SPI->PSEL.SCK = NRF_GPIO_PIN_MAP(EXPANDER_SCK_PORT_NUMBER,EXPANDER_SCK_PIN);
        
    EXPANDER_SPI->PSEL.MOSI = NRF_GPIO_PIN_MAP(EXPANDER_MOSI_PORT_NUMBER,EXPANDER_MOSI_PIN );
    EXPANDER_SPI->PSEL.MISO = NRF_GPIO_PIN_MAP(EXPANDER_MISO_PORT_NUMBER,EXPANDER_MISO_PIN );
        
    EXPANDER_SPI->FREQUENCY = SPI_FREQUENCY_FREQUENCY_M4;
    EXPANDER_SPI->CONFIG = 0;
           
    EXPANDER_SPI->ENABLE = 1;
}


uint8_t Expander_SPI_RW(uint8_t DataOut)
{
    uint8_t Junk;
    Junk = EXPANDER_SPI->RXD;
    EXPANDER_SPI->EVENTS_READY = 0;
    EXPANDER_SPI->TXD = DataOut;

    while(EXPANDER_SPI->EVENTS_READY == 0)
    {
    }

    EXPANDER_SPI->EVENTS_READY = 0;
    return EXPANDER_SPI->RXD;
}