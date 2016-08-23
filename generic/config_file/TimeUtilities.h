/*
 * TimeUtilities.h
 *
 *  Created on: Jun 5, 2014
 *      Author: emh203
 */

#ifndef TIMEUTILITIES_H_
#define TIMEUTILITIES_H_

#define MAX_TIME_DATE_STRING_LENGTH	32

extern uint8_t TimeDateString_8601[MAX_TIME_DATE_STRING_LENGTH];

uint32_t MakeISO_8601_Time(tm * T,uint8_t *Buffer);

uint32_t MakeISO_8601_Date(tm * T,uint8_t *Buffer);

uint32_t MakeISO_8601_TimeDate(tm * T,uint8_t *Buffer);

#endif /* TIMEUTILITIES_H_ */
