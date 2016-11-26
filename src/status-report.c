#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>
#include <math.h>

#include "Drivers/rpi_base.h"
#include "Drivers/rpi_i2c.h"
#include "Drivers/rpi_spi_slave.h"
#include "Drivers/accel_driver.h"
#include "Drivers/hc_sr04_driver.h"

#include "trace.h"

extern uint32_t GET_SP();

static char buf[512];

static char to_hex(uint8_t v){
    if (/*0 <= v &&*/ v <= 9) return '0'+v;
    else return 'A' + v - 10;
}

static void out_hex_buf(const uint8_t* const buf){
    uint32_t i;
    
    for(i = 0; i < 32; ++i){
        uart_char(to_hex((buf[i] >> 4) & 0x0F));
        uart_char(to_hex(buf[i] & 0x0F));
        uart_char(' ');
        if(15 == i) uart_str("\n\r");
    }
    uart_str("\n\r\n\r");
}

void status_task(void *pParam){
    int i;
    TickType_t tick;
    int16_t acc_data[3];
    int16_t gyro_data[4];
    float sonar_data[4];
    float acc_module, ax = 0.0f, ay = 0.0f, az = 0.0f;
    float temp, gx,gy,gz;
    uint32_t seconds;
    uint32_t irq_sp, sp;
    uint32_t spi_fr;

    portTASK_USES_FLOATING_POINT();
    
    static const TickType_t delay = 500 * portTICK_PERIOD_MS;

    uart_strln("Status task started");


    //vTaskDelay(15000);

    for(;;){
        tick = xTaskGetTickCount();

        get_accel_raw(acc_data);

        seconds = tick / 1000 / portTICK_PERIOD_MS;
        sprintf(buf, "\033[2JTime from start - %ld sec.  %ld days %02ld:%02ld:%02ld\n\r",
                seconds, seconds/86400, (seconds/3600)%24, (seconds/60)%60, seconds % 60);
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

        sprintf(buf, "Acc driver status \n\r irq: %6ld, driver: %6ld, restarts: %6ld\n\r",
                get_irq_count(), get_driver_count(), get_driver_restarts());
        uart_str(buf);

        for(i = 0; i < 4; ++i){
            sonar_data[i] = sonar_get_distance(i);
        }

        sprintf(buf, "Sonar data -- 1: %7.1f cm, 2: %7.1f cm, 3: %7.1f cm, 4: %7.1f cm",
                sonar_data[0], sonar_data[1], sonar_data[2], sonar_data[3]);
        uart_strln(buf);
        uart_str("\n\r");

        spi_fr = BSC_SL->FR;
        sprintf(buf, "BSC_SL RX FIFO start level: %lu, RXFE: %lu, RXFF: %lu\n\r",
                (spi_fr & BSC_SL_FR_RXLVL_MASK) >> BSC_SL_FR_RXLVL_SHIFT,
                spi_fr & BSC_SL_FR_RXFE, spi_fr & BSC_SL_FR_RXFF);
        uart_str(buf);
        sprintf(buf, "BSC_SL TX FIFO start level: %lu, TXFE: %lu, TXFF: %lu\n\r",
                (spi_fr & BSC_SL_FR_TXLVL_MASK) >> BSC_SL_FR_TXLVL_SHIFT,
                spi_fr & BSC_SL_FR_TXFE, spi_fr & BSC_SL_FR_TXFF);
        uart_str(buf);
        sprintf(buf, "BSC_SL FR: %lx, irq: %lu\n\r",
                spi_fr, spi_slave_irq());
        uart_str(buf);

        uart_strln("SPI buffer:");
        out_hex_buf(get_spi_buffer());
        
        vTaskList(buf);
        uart_strln(buf);

        sp = GET_SP();
        irq_sp = get_sonar_sp();

        sprintf(buf, "Stack: %8lx, IRQ stack: %8lx\n\r", sp, irq_sp);
        uart_str(buf);

        vTaskDelayUntil(&tick, delay);
    }

    vTaskDelete(NULL);

    (void) pParam;
}
