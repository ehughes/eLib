/*
 * LPC43xx_ProgrammingDriver.h
 *
 *  Created on: May 17, 2015
 *      Author: ehughes
 */

#include "iHex.h"

#ifndef LPC43XX_PROGRAMMINGDRIVER_H_
#define LPC43XX_PROGRAMMINGDRIVER_H_


#define LPC43xx_ADDRESS_NOT_IN_FLASH_BANK	0xFFFFFFFF
#define LPC43xx_FLASH_BANK_A				0
#define LPC43xx_FLASH_BANK_B				1


#define LPC43xx_PROGRAMMING_OK							0
#define LPC43xx_PROGRAMMING_ERROR_CACHE_MISALIGNMENT	1
#define LPC43xx_PROGRAMMING_ERROR_NOT_IN_VALID_RANGE	2
#define LPC43xx_PROGRAMMING_ERROR_PREPARE_SECTOR		3


//We need to implement a cacheing mechanism to program the flash as we have to program in 512 or 1024 byte Blocks

#define LPC43xx_FLASH_CACHE_SIZE					1024
#define LPC43xx_COMPUTE_FLASH_CACHE_BASE(x)			(x & (~(LPC43xx_FLASH_CACHE_SIZE-1)))
#define LPC43xx_COMPUTE_FLASH_CACHE_OFFSET(x)		(x & (LPC43xx_FLASH_CACHE_SIZE-1))


uint32_t LPC43xx_FLASH_Program_iHexLine(iHEX_LineInfo * LI);
uint32_t LPC43xx_FLASH_FlushCache() ;

#endif /* LPC43XX_PROGRAMMINGDRIVER_H_ */
