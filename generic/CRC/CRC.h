/* 
 * File:   CRC.h
 * Author: ajm5177
 *
 * Created on February 16, 2016, 4:35 PM
 */

#ifndef CRC_H
#define	CRC_H
#include "..\System.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define CRC_SEED 0xEDB88320
    
unsigned int Generate32bitCRC(char* data, int16_t len, uint32_t seed);

#ifdef	__cplusplus
}
#endif

#endif	/* CRC_H */

