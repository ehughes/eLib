#include "MasterConfiguration.h"
#include "Board.h"

#if (BOARD_HAS_I2C == 1) && (ENABLE_BME280 == 1)

#include "BME280.h"
#include "bme280_defs.h"

#ifndef  _BME280_INT
#define _BME280_INT

void InitBM280();
extern struct bme280_dev MyBME280;

#endif

#endif