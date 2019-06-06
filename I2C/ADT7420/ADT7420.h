#include "MasterConfiguration.h"

#if (BOARD_HAS_I2C == 1) && (ENABLE_ADT7420 == 1)

#ifndef __ADT1014_H
#define __ADT1014_H



void ADT7420_Init(uint8_t AD7420_Address);
float ADT7420_GetTemperature(uint8_t AD7420_Address);

#define ADT7420_BOARD_ADDRESS           (0x49)

#ifdef WIRED_INAD1015
        #define ADT7420_TRANSDUCER_ADDRESS  (0x48)
#else
        #define ADT7420_TRANSDUCER_ADDRESS  (0x4B)
#endif

#endif



#endif
