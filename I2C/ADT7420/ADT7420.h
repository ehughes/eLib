#include "MasterConfiguration.h"

#ifndef __ADT1014_H
#define __TDC_DumpConfigADT1014_H

void ADT7420_Init(uint8_t AD7420_Address);

bool ADT7420_GetTemperature(uint8_t AD7420_Address,float *Temperature);

#define ADT7420_BOARD_ADDRESS 		(0x49)

#ifdef WIRED_INAD1015
	#define ADT7420_TRANSDUCER_ADDRESS  (0x48)
#else
	#define ADT7420_TRANSDUCER_ADDRESS  (0x4B)
#endif






#endif
