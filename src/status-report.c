#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>
#include <math.h>

#include "Drivers/rpi_base.h"
#include "Drivers/rpi_i2c.h"
#include "Drivers/rpi_spi_slave.h"
#include "Drivers/accel_driver.h"

#include "trace.h"

static char buf[512];

void status_task(void *pParam){
    TickType_t tick;
    int16_t acc_data[3];
    int16_t gyro_data[4];
    float acc_module, ax = 0.0f, ay = 0.0f, az = 0.0f;
    float temp, gx,gy,gz;
    //uint32_t i = 0, spi_fr;
    
    static const TickType_t delay = 500 * portTICK_PERIOD_MS;

    uart_strln("Status task started");

    vTaskDelay(6*delay);
    
    for(;;){
        tick = xTaskGetTickCount();

        get_accel_raw(acc_data);

        sprintf(buf, "\033[2JTime from start: %ld sec.\n\r",
                tick / 1000 / portTICK_PERIOD_MS);
        uart_str(buf);

        sprintf(buf, "Accelerometer raw data x: %6d, y: %6d, z:%6d.\n\r",
                acc_data[0], acc_data[1], acc_data[2]);
        uart_str(buf);

        //ax = (float)acc_data[0]/256.0;
        //ay = (float)acc_data[1]/256.0;
        //az = (float)acc_data[2]/256.0;
        ax = (float)acc_data[0]*0.004;
        ay = (float)acc_data[1]*0.004;
        az = (float)acc_data[2]*0.004;
        
        acc_module = sqrt(ax*ax + ay*ay +az*az);
        
        sprintf(buf, "Accelerometer data -- x: %7.4f, y: %7.4f, z:%7.4f, abs: %7.4f\n\r",
                ax, ay, az, acc_module);
        uart_str(buf);

        get_turn_rate_raw(gyro_data+1);
        gyro_data[0] = get_temp_raw();

        sprintf(buf, "Gyro raw data -- temp: %d, x: %d, y:%d, z: %d\n\r",
                gyro_data[0], gyro_data[1], gyro_data[2], gyro_data[3]);
        uart_str(buf);


        temp = (float)(gyro_data[0]+13200)/280.0 + 35.0;
        gx = (float)gyro_data[1] / 14.375;
        gy = (float)gyro_data[2] / 14.375;
        gz = (float)gyro_data[3] / 14.375;
        sprintf(buf, "Gyro -- temp: %7.4f, x: %7.4f, y:%7.4f, z: %7.4f\n\r",
                temp, gx,gy,gz);
        uart_str(buf);

        sprintf(buf, "Acc driver status -- irq: %6ld, driver: %6ld\n\r",
                get_irq_count(), get_driver_count());
        uart_str(buf);

        vTaskList(buf);
        uart_strln(buf);


        /*
        //if(0 == i){
            BSC_SL->FR = 0;
            BSC_SL->CR = BSC_SL_CR_EN | BSC_SL_CR_SPI | BSC_SL_CR_BRK | BSC_SL_CR_TXE;
            i = 0;
            //vTaskDelay(10);
            
            spi_fr = BSC_SL->FR;
            sprintf(buf, "BSC_SL TX FIFO start level: %lu, TXFE: %lu, TXFF: %lu\n\r",
                    (spi_fr & BSC_SL_FR_TXLVL_MASK) >> BSC_SL_FR_TXLVL_SHIFT,
                    spi_fr & BSC_SL_FR_TXFE, spi_fr & BSC_SL_FR_TXFF);
            uart_str(buf);

            //vTaskDelay(10);
            do {
                BSC_SL->DR = i & 0xFF;
                ++i;
                //vTaskDelay(10);
                spi_fr = BSC_SL->FR;
            } while (!(spi_fr & BSC_SL_FR_TXFF));
            //}
        
        sprintf(buf, "BSC_SL TX FIFO full level: %lu, number of writes: %lu\n\r",
                ((spi_fr & BSC_SL_FR_TXLVL_MASK) >> BSC_SL_FR_TXLVL_SHIFT), i);
        uart_str(buf);

        sprintf(buf, "BSC_SL RX FIFO level: %lu\n\r",
                ((spi_fr & BSC_SL_FR_RXLVL_MASK) >> BSC_SL_FR_RXLVL_SHIFT));
        uart_str(buf);

        sprintf(buf, "BSC_SL DR: %lx, FR: %lx\n\r", BSC_SL->DR, spi_fr);
        uart_str(buf);
        */

        vTaskDelayUntil(&tick, delay);
    }

    vTaskDelete(NULL);

    (void) pParam;
}
