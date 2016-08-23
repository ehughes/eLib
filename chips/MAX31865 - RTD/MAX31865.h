#include <stdint.h>

/*
 * File:   MAX31865.h
 * Author: emh203
 *
 * Created on November 20, 2013, 9:47 AM
 */

#ifndef MAX31865_H
#define	MAX31865_H

void MAX31865_Init();
uint8_t MAX31865_DataReady();
void MAX31865_PowerOn();
void MAX31865_PowerOff();
int16_t MAX31865_GetIntegerTemperature();
float MAX31865_GetTemperature();
void MAX31865_Configure();

#define MAX31865_REG_CONFIGURATION_READ           0x00
#define MAX31865_REG_RTD_MSB_READ                 0x01
#define MAX31865_REG_RTD_LSB_READ                 0x02
#define MAX31865_REG_HIGH_FAULT_MSB_READ          0x03
#define MAX31865_REG_HIGH_FAULT_LSB_READ          0x04
#define MAX31865_REG_LOW_FAULT_MSB_READ           0x05
#define MAX31865_REG_LOW_FAULT_LSB_READ           0x06
#define MAX31865_REG_FAULT_STATUS_READ            0x07

#define MAX31865_REG_CONFIGURATION_WRITE        (MAX31865_REG_CONFIGURATION_READ  | 0x80)
#define MAX31865_REG_HIGH_FAULT_MSB_WRITE       (MAX31865_REG_HIGH_FAULT_MSB_READ | 0x80)
#define MAX31865_REG_HIGH_FAULT_LSB_WRITE       (MAX31865_REG_HIGH_FAULT_LSB_READ | 0x80)
#define MAX31865_REG_LOW_FAULT_MSB_WRITE        (MAX31865_REG_LOW_FAULT_MSB_READ  | 0x80)
#define MAX31865_REG_LOW_FAULT_LSB_WRITE        (MAX31865_REG_LOW_FAULT_LSB_READ  | 0x80)


#define RTD_TYPE_100_EUROPEAN_ALPHA_003850   0
#define RTD_TYPE_1000_EUROPEAN_ALPHA_003850  1
#define RTD_TYPE_100_AMERICAN_ALPHA_003920   2
#define RTD_TYPE_1000_AMERICAN_ALPHA_003920  3


#endif	/* MAX31865_H */

