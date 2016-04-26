#include <FreeRTOS.h>
#include <task.h>
#include "rpi_i2c.h"
#include "i2c_itg3200.h"

void itg3200_init(void){
    rpi_i2c_set_reg(ITG3200_ADDR, ITG3200_PWR_MGM,
                    ITG3200_PWR_MGM_H_RESET);
    vTaskDelay(50);
    rpi_i2c_set_reg(ITG3200_ADDR, ITG3200_PWR_MGM,
                    ITG3200_PWR_MGM_CLK_PLL_XG);
    rpi_i2c_set_reg(ITG3200_ADDR, ITG3200_DLPF_FS,
                    ITG3200_DLPF_CFG_188HZ | ITG3200_FS_SEL_2000);
        rpi_i2c_set_reg(ITG3200_ADDR, ITG3200_SMPLT_DIV, 9);
}

void itg3200_read_data(int16_t data[4]){
    uint8_t raw[8];

    rpi_i2c_read(ITG3200_ADDR, ITG3200_TEMP_OUT_H, 8, raw);

    data[0] = (raw[0] << 8) | raw[1];
    data[1] = (raw[2] << 8) | raw[3];
    data[2] = (raw[4] << 8) | raw[5];
    data[3] = (raw[6] << 8) | raw[7];
}
