#ifndef CRC_H
#define CRC_H

#include "stdint.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CRC_SEED 0xEDB88320
    
unsigned int Generate32bitCRC(char* data, int16_t len, uint32_t seed);


uint16_t CRC16_CCIT_Step(uint16_t CRC_In, uint8_t Data);


uint16_t CRC16_CCIT(uint8_t *Data, uint32_t Length);

#ifdef  __cplusplus
}
#endif

#endif  /* CRC_H */

