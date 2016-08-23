/*
 * DiskManager.h
 *
 *  Created on: May 23, 2014
 *      Author: ehughes
 */
#include ".\fatfs\diskio.h"
#include ".\fatfs\ff.h"

#ifndef DISKMANAGER_H_
#define DISKMANAGER_H_

extern FATFS MySD_FatFs;
extern FATFS MyUSB_FatFs;

void InitFileSystem();
void ProcessFileSystem();


//THis Variable must be incremented by a system timer.   It is scaled to 1mS per bit
extern volatile uint32_t DiskManagerTicker;

void MSC_Enable();
void MSC_Disable();

uint8_t MSC_IsEnabled();



#define DRIVE_SD		0
#define DRIVE_USB		1

#endif /* DISKMANAGER_H_ */
