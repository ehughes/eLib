/*
 * System.h
 *
 *  Created on: Apr 28, 2014
 *      Author: ehughes
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

/***
 *      _   _                _
 *     | | | | ___  __ _  __| | ___ _ __ ___
 *     | |_| |/ _ \/ _` |/ _` |/ _ \ '__/ __|
 *     |  _  |  __/ (_| | (_| |  __/ |  \__ \
 *     |_| |_|\___|\__,_|\__,_|\___|_|  |___/
 *
 */

#include "FIRMWARE_VERSION.h"
#include "Board.h"
#include "Chip.h"
#include <stdint.h>
//File System
#include "FILE_SYSTEM\FileSystem.h"
#include <stdarg.h>
#include "ihex.h"
#include "string.h"
#include "LPC43xx_ProgrammingDriver.h"
#include "DeviceProgramming.h"

void EmitMsg(const char *FormatString,...);


void InitSystemTimer();
void Delay_mS(uint32_t mS_TicksToDelay);

#define NUM_GP_TIMERS 16
extern volatile uint32_t GP_Timer[NUM_GP_TIMERS];

#define BOOT_TIMER					GP_Timer[0]
#define DISK_MANAGER_TICKER			GP_Timer[1]
#define DRIVE_CHECK_TIMER			GP_Timer[2]

/***
 *      ____ _____  _  _____ _____
 *     / ___|_   _|/ \|_   _| ____|
 *     \___ \ | | / _ \ | | |  _|
 *      ___) || |/ ___ \| | | |___
 *     |____/ |_/_/   \_\_| |_____|
 *
 */

#define SYSTEM_STATE_BOOT						0x00


uint8_t ChangeSystemState(uint8_t NextState);
uint8_t GetSystemState();


#endif /* SYSTEM_H_ */
