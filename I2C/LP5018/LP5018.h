#include "stdint.h"

#ifndef _LP5018_H
#define _LP5018_H


#define LP5018_I2C_ADDRESS (0x28)


typedef struct
{
    uint8_t Brightness;
    uint8_t Red;
    uint8_t Green;
   
    uint8_t Blue;

} LP5018_LED;


#define NUM_LP5018_LEDS    6 

extern LP5018_LED MyLP5018_LED[NUM_LP5018_LEDS];


void LP5018_Init();
void LP5018_SetLED_Brightness(uint8_t LED_Number, uint8_t Brightness);
void LP5018_SetLED_Color(uint8_t LED_Number, LP5018_LED *LED);
void LP5018_SetLED(uint8_t LED_Number, LP5018_LED *LED);

extern const uint8_t GlowTable[256];
#endif
