#include "MasterConfiguration.h"
#include "Board.h"


#if (BOARD_HAS_I2C == 1) && (ENABLE_BME280 == 1)

#include "System_I2C.h"
#include "System.h"
#include "BME280.h"

int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

struct bme280_dev MyBME280;



void InitBM280()
{
    int8_t rslt;
    uint8_t settings_sel;
    struct bme280_data comp_data;

    MyBME280.dev_id = BME280_I2C_ADDR_PRIM;
    MyBME280.intf = BME280_I2C_INTF;
    MyBME280.read = user_i2c_read;
    MyBME280.write = user_i2c_write;
    MyBME280.delay_ms = Delay_mS;

    bme280_init(&MyBME280);

/* Recommended mode of operation: Indoor navigation */
    MyBME280.settings.osr_h = BME280_OVERSAMPLING_1X;
    MyBME280.settings.osr_p = BME280_OVERSAMPLING_16X;
    MyBME280.settings.osr_t = BME280_OVERSAMPLING_2X;
    MyBME280.settings.filter = BME280_FILTER_COEFF_16;
    MyBME280.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    rslt = bme280_set_sensor_settings(settings_sel, &MyBME280);
    rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &MyBME280);

}


int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
   

    return System_I2C_TxRxRaw(dev_id,&reg_addr,1,reg_data,len);
     
    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Stop       | -                   |
     * | Start      | -                   |
     * | Read       | (reg_data[0])       |
     * | Read       | (....)              |
     * | Read       | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */
   
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    if(len>255)
        return -1;

     uint8_t Temp[256];
    
    Temp[0] = reg_addr;

    memcpy(&Temp[1],reg_data,len);

    System_I2C_WriteRaw(dev_id,&Temp[0],len+1);

    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */

    return rslt;
}

#endif
