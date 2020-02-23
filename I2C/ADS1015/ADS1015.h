/*
 * ADS1014.h
 *
 *  Created on: Apr 4, 2018
 *      Author: ehugh
 */
#include "stdint.h"

#ifndef SYSTEM_DRIVERS_I2C_ADS1014_ADS1014_H_
#define SYSTEM_DRIVERS_I2C_ADS1014_ADS1014_H_


void ADS1015_SelectChannel(uint8_t C);

void ADS1015_Init();

int16_t ADS1015_Read();

uint16_t ADS1015_ReadConfig();

#endif /* SYSTEM_DRIVERS_I2C_ADS1014_ADS1014_H_ */
