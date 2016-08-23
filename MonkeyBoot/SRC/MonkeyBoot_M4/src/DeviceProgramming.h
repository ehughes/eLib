/*
 * DeviceProgramming.h
 *
 *  Created on: May 17, 2015
 *      Author: ehughes
 */

#ifndef DEVICEPROGRAMMING_H_
#define DEVICEPROGRAMMING_H_


#define PROGRAM_OK						0
#define PROGRAM_ERROR_NOT_IN_RANGE		1
#define PROGRAM_ERROR_UNKNOWN_ALGORITHM	2
#define PROGRAM_ERROR					3

uint32_t ProgramLine(iHEX_LineInfo *LI);
uint32_t FinalizeProgramming();

#endif /* DEVICEPROGRAMMING_H_ */
