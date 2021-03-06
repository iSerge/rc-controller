#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <string.h>

#include "rpi_irq.h"
#include "rpi_gpio.h"
#include "rpi_i2c.h"
#include "i2c_adxl345.h"
#include "i2c_itg3200.h"
#include "rpi_systimer.h"

#include "accel_driver.h"

#include "trace.h"

static TaskHandle_t xHandlingTask = NULL;

static void driver_task(void *pParam);
static void accelISR();

void init_accel_driver(){
    BaseType_t xReturned;
    uart_strln("\033[2JStaring accel driver task");
    xReturned = xTaskCreate(driver_task, "Accel driver", 1024,
                            NULL, configMAX_PRIORITIES - 2, &xHandlingTask);
    if(pdPASS == xReturned){
        uart_strln("Driver task created");
    } else {
        uart_strln("Driver task creation failed");
    }
}

static uint32_t irq_count = 0, driver_count = 0, driver_restarts = 0;
static BaseType_t xHigherPriorityTaskWoken;

static void accelISR(void *pParam)
{
/* Clear the interrupt. */
    rpi_gpio_ev_clear_status(17);

    ++irq_count;

    xHigherPriorityTaskWoken = pdFALSE;

    if (NULL == xHandlingTask){
        uart_strln("No task to notify");
    } else {
        vTaskNotifyGiveFromISR( xHandlingTask, &xHigherPriorityTaskWoken );
    }
    
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    
    (void) pParam;
}

static int16_t accel_raw[3] = {0, 0, 0};
static int16_t gyro_data_raw[4] = {0, 0, 0, 0};

static int16_t accel_data[3] = {0, 0, 0};
static int16_t gyro_data[4] = {0, 0, 0, 0};

static void init_adxl345(void){
    adxl345_init();
    //itg3200_init();

    rpi_gpio_register_ev_handler(17, accelISR, NULL);

    rpi_gpio_ev_detect_enable(17, GPIO_EV_STATUS | GPIO_EV_RISING_EDGE);

    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_INT_MAP, 0);
    rpi_i2c_set_reg(ADXL345_ADDR, ADXL345_INT_ENABLE, ADXL345_INT_DATA_READY);
}

static void driver_task(void *pParam){
    portTASK_USES_FLOATING_POINT();

    init_adxl345();
    
    rpi_irq_enable(RPI_IRQ_ID_GPIO_0);

    for(;;){
        if(pdFALSE == xTaskNotifyWait( 0, 0, 0, pdMS_TO_TICKS(1000) )){
            ++driver_restarts;
            init_adxl345();
            continue;
        }

        ++driver_count;
        adxl345_read_axes(accel_raw);
        //itg3200_read_data(gyro_data_raw);
        
        accel_data[0] = accel_data[0] * 0.8 + 0.2 * accel_raw[0];
        accel_data[1] = accel_data[1] * 0.8 + 0.2 * accel_raw[1];
        accel_data[2] = accel_data[2] * 0.8 + 0.2 * accel_raw[2];
        
        //gyro_data[0] = gyro_data[0] * 0.9 + 0.1 * gyro_data_raw[0];
        //gyro_data[1] = gyro_data[1] * 0.8 + 0.2 * gyro_data_raw[1];
        //gyro_data[2] = gyro_data[2] * 0.8 + 0.2 * gyro_data_raw[2];
        //gyro_data[3] = gyro_data[3] * 0.8 + 0.2 * gyro_data_raw[3];
    }

    vTaskDelete( NULL );
    (void) pParam;
}

void get_accel_raw(int16_t acc[3]){
    memcpy(acc, accel_data, 3*sizeof(int16_t));
}

void get_accel(float acc[3]);

void get_turn_rate_raw(int16_t t[3]){
    memcpy(t, gyro_data + 1, 3*sizeof(int16_t));
}
    
void get_turn_rate(float t[3]);

int16_t get_temp_raw(){
    return gyro_data[0];
}

float get_temp(){
    return (float)(gyro_data_raw[0] + 13200) / 280.0 + 35.0;
}

uint32_t get_irq_count(){
    return irq_count;
}

uint32_t get_driver_count(){
    return driver_count;
}

uint32_t get_driver_restarts(){
    return driver_restarts;
}

