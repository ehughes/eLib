/*
 * MonkeBootConfig.h
 *
 *  Created on: Oct 30, 2014
 *      Author: emh203
 */

#ifndef MONKEYBOOTCONFIG_H_
#define MONKEYBOOTCONFIG_H_

#define BOOT_DRIVE	DRIVE_SD

#define BOOT_IMAGE_NAME				"boot.hex"
#define BOOT_IMAGE_NAME_PROGRAMMED	"boot.hex.prg"


typedef struct {

	uint32_t Start;
	uint32_t Length;
	uint32_t Type;

} MemoryRange;


#define NUM_BOOTLOADER_MEMORY_RANGES	1
#define NUM_PROGRAMMABLE_MEMORY_RANGES	2

extern const MemoryRange MyBootloaderMemoryRanges[NUM_BOOTLOADER_MEMORY_RANGES];
extern const MemoryRange MyProgrammableMemoryRanges[NUM_PROGRAMMABLE_MEMORY_RANGES];

#define MEM_RANGE_LPC43xx_INT_FLASH			0
#define MEM_RANGE_LPC43xx_INT_INTRAM		1
#define MEM_RANGE_LPC43xx_INT_EXT_SDRAM		2
#define MEM_RANGE_LPC43xx_INT_EXT_FLASH		3






#endif /* MONKEYBOOTCONFIG_H_ */
