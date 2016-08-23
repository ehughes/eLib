#include <plib.h>
#include "MAX31865.h"
#include "LUT.h"
#include "RTD_Table.h"


/***
 *      _   _    _    _
 *     | | | |  / \  | |
 *     | |_| | / _ \ | |
 *     |  _  |/ ___ \| |___
 *     |_| |_/_/   \_\_____|
 *
 */

#define MAX31865_CS_ACTIVE       PORTEbits.RE4 = 0;
#define MAX31865_CS_INACTIVE     PORTEbits.RE4 = 1;


void MAX31865_Init()
{
    TRISEbits.TRISE3 = 1; //TEMP_DRDY
    TRISEbits.TRISE4 = 0; //RTD_CS
    MAX31865_CS_INACTIVE;
    TRISGbits.TRISG12 = 0; //2 pins are parallelled for power control to the temperature sensor
    TRISGbits.TRISG13 = 0;
    MAX31865_PowerOn();
}

uint8_t MAX31865_DataIsReady()
{
    if(PORTEbits.RE3 == 0)
        return 1;
    else
        return 0;
}

uint8_t MAX31865_SPI_ReadWrite(uint8_t DataOut)
{
    SPI2BUF = DataOut;

    while(SPI2STATbits.SPIRBF == 0) {}

    return SPI2BUF;
}

void MAX31865_PowerOn()
{
    PORTGSET = (1<<12) | (1<<13);
}

void MAX31865_PowerOff()
{
    PORTGCLR = (1<<12) | (1<<13);
}



/***
 *      ____  _                 _     _
 *     |  _ \| |_   _ _ __ ___ | |__ (_)_ __   __ _
 *     | |_) | | | | | '_ ` _ \| '_ \| | '_ \ / _` |
 *     |  __/| | |_| | | | | | | |_) | | | | | (_| |
 *     |_|   |_|\__,_|_| |_| |_|_.__/|_|_| |_|\__, |
 *                                            |___/
 */


static uint8_t RTD_Type = RTD_TYPE_100_AMERICAN_ALPHA_003920;
static float RTD_ReferenceResistor = 400.0;
static LUT_F *MyRTDTable = (LUT_F *) &RTD_American_100Ohm_Alpha003920;
float ResistancePrescale;  //Tables are in 100 form.   We need this to prescale 1000 Temp data;

uint8_t MAX31865_ReadRegister(uint8_t Address)
{
    uint8_t Data;
    MAX31865_CS_ACTIVE;
    MAX31865_SPI_ReadWrite(Address); //Send out the Address
    Data = MAX31865_SPI_ReadWrite(0); //Get the Data
    MAX31865_CS_INACTIVE;
    return Data;
}


void MAX31865_WriteRegister(uint8_t Address, uint8_t Data)
{
    MAX31865_CS_ACTIVE;
    MAX31865_SPI_ReadWrite(Address); //Send out the Address
    MAX31865_SPI_ReadWrite(Data); //Send the Data
    MAX31865_CS_INACTIVE;
}


/*
   Function: MAX31865_GetIntegerTemperature

 Gets the temperature from the MAX31865.   It assumes a 400ohm reference resistor.
 This function uses the ADC CODE/32-256  approximation described on page 19 of the MAX3185 datasheet 19-6478; Rev 0; 10/12.

   Parameters:

 None

   Returns:

    A signed integer representing termperature in degrees C.  Scaling is 1C per bit.

*/

int16_t MAX31865_GetIntegerTemperature()
{
    InitSPI2_ForMAX31865();
    uint16_t T;
    T = (uint16_t)(MAX31865_ReadRegister(MAX31865_REG_RTD_MSB_READ))<<8;
    T |= (uint16_t)(MAX31865_ReadRegister(MAX31865_REG_RTD_LSB_READ));
    //Shift by one to get the 15 value.  Then do the divide by 32 substract by 256
    return (((int16_t)T)>>6)-256;
}

void MAX31865_Configure()
{
    InitSPI2_ForMAX31865();
    //Set VBIAS On, Conversion Mode Auto, 3 wire mode 60Hz filter.
    MAX31865_WriteRegister(MAX31865_REG_CONFIGURATION_WRITE , 0xd0);
}



void MAX31865_SetupRTD(float ReferenceResistor,uint8_t Type)
{
    RTD_ReferenceResistor = ReferenceResistor;
    RTD_Type = Type;

    switch(RTD_Type)
        {
            default:
            case RTD_TYPE_100_EUROPEAN_ALPHA_003850   :
                ResistancePrescale = 1.0;
                MyRTDTable = (LUT_F *)&RTD_European_100Ohm_Alpha003850;
                break;

            case RTD_TYPE_1000_EUROPEAN_ALPHA_003850  :
                ResistancePrescale = 0.1;
                MyRTDTable = (LUT_F *)&RTD_European_100Ohm_Alpha003850;
                break;

            case RTD_TYPE_100_AMERICAN_ALPHA_003920   :
                ResistancePrescale = 1.0;
                MyRTDTable = (LUT_F *)&RTD_American_100Ohm_Alpha003920;
                break;

            case RTD_TYPE_1000_AMERICAN_ALPHA_003920  :
                ResistancePrescale = 0.1;
                MyRTDTable = (LUT_F *)&RTD_American_100Ohm_Alpha003920;
                break;
        }
}

float MAX31865_GetResistance()
{
    InitSPI2_ForMAX31865();
    uint16_t T;
    T = (uint16_t)(MAX31865_ReadRegister(MAX31865_REG_RTD_MSB_READ))<<8;
    T |= (uint16_t)(MAX31865_ReadRegister(MAX31865_REG_RTD_LSB_READ));
    return ((float)(T>>1)/32768.0) * RTD_ReferenceResistor;
}

float MAX31865_GetTemperature()
{
    float R;
    R = MAX31865_GetResistance();
    R = R * ResistancePrescale;
    return Interplotate_LUT_F(MyRTDTable , R);
}
