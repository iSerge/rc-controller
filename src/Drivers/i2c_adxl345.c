#include "i2c_adxl345.h"


void adxl345_init(){
    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_POWER_CTL, 0);
    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_POWER_CTL,
                    ADXL345_LINK | ADXL345_MEASURE);

    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_INT_ENABLE, 0);

    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_FIFO_CTL,
                    ADXL345_FIFO_MODE_BYPASS | 16);

    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_BW_RATE, ADXL345_DR_100_Hz);

    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_DATA_FORMAT, 0);
    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_DATA_FORMAT, 
                    ADXL345_FULL_RES | ADXL345_RANGE_16G);
}

I2C_Status adxl345_read_axes(int16_t data[3]){
    I2C_Status err = rpi_i2c_read(ADXL345_ADDR, ADXL345_DATAX0, 6,
                                  (unsigned char*)data);

    return err;
}
