
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "rpi_gpio.h"
#include "rpi_irq.h"
#include "rpi_systimer.h"
#include "hc_sr04_driver.h"
#include "trace.h"

static void sonarISR(void *pParam);
static void sonar_driver_task(void *pParam);

static QueueHandle_t sonar_data_q = NULL;

#define SSPEED_CM_S 34320.0

#define MAX_SONARS 4
static float sonar_data[MAX_SONARS] = {0.0, 0.0, 0.0, 0.0};
static uint32_t id2pin[MAX_SONARS] = {7, 8, 25, 24};
static uint32_t id2echo[MAX_SONARS] = {23, 9, 10, 22};

extern uint32_t GET_SP();
static uint32_t sp = 0;

uint32_t get_sonar_sp(){
    return sp;
}

void init_sonar_driver(){
    uint32_t i;

    for(i = 0; i < MAX_SONARS; ++i){
        rpi_gpio_sel_fun(id2pin[i], GPIO_FSEL_OUT);
        rpi_gpio_set_val(id2pin[i], 0);

        rpi_gpio_sel_fun(id2echo[i], GPIO_FSEL_IN);
        rpi_gpio_register_ev_handler(id2echo[i], sonarISR, (void*)i);
    }

    sonar_data_q = xQueueCreate(16, sizeof(uint32_t));
    if(NULL == sonar_data_q){
        uart_strln("Sonar queue creation failed");
    } else {
        uart_strln("Sonar queue created");
    }
    
    BaseType_t xReturned;
    uart_strln("Staring sonar driver task");
    xReturned = xTaskCreate(sonar_driver_task, "Sonar driver", 512,
                            NULL, configMAX_PRIORITIES - 1, NULL);
    if(pdPASS == xReturned){
        uart_strln("Sonar driver task created");
    } else {
        uart_strln("Sonar driver task creation failed");
    }
}

static void sonarISR(void *pParam){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    static uint64_t start;
    static uint64_t now;
    static uint32_t delta;
    static uint32_t echo;
    now = rpi_sys_timer_get64();

    sp = GET_SP();
    
    echo = id2echo[(uint32_t)pParam];

    rpi_gpio_ev_clear_status(echo);

    if(rpi_gpio_get_val(echo)){
        start = now;
    } else {
        delta = (uint32_t)((now - start)&0xFFFFFFFF);
        xQueueSendFromISR(sonar_data_q,
                   &delta,
                   &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    (void)pParam;
}

void microDelay(uint32_t useconds){
    uint64_t time = rpi_sys_timer_get64() + useconds;
    while(rpi_sys_timer_get64() < time){}
}

void runMeasureCycle(int sensor_id){
    static uint32_t echo_time;
    static BaseType_t qResult;

    uint32_t pin = id2pin[sensor_id];
    uint32_t echo = id2echo[sensor_id];

    rpi_gpio_ev_detect_enable(echo, GPIO_EV_RISING_EDGE | GPIO_EV_FALLING_EDGE);
    //vTaskDelay(1);

    xQueueReset(sonar_data_q);
    
    rpi_gpio_set_val(pin, 1);
    microDelay(10);
    //vTaskDelay(1);
    rpi_gpio_set_val(pin, 0);

    qResult = xQueueReceive(sonar_data_q, &echo_time, 50);
    if(pdTRUE == qResult){
        sonar_data[sensor_id] = (float)echo_time / 1000000.0 * SSPEED_CM_S * 0.5435;
    } else {
        sonar_data[sensor_id] = -1.0;
    }

    rpi_gpio_ev_detect_disable(echo, GPIO_EV_RISING_EDGE | GPIO_EV_FALLING_EDGE);

}

void sonar_driver_task(void *pParam){
    int i = 0;

    portTASK_USES_FLOATING_POINT();
    
    rpi_irq_enable(RPI_IRQ_ID_GPIO_0);
   
    while(1){
        for(i = 0; i < MAX_SONARS; ++i){
            runMeasureCycle(i);
            vTaskDelay(10);
        }
    }
    
    vTaskDelete(NULL);
    (void)pParam;
}

float sonar_get_distance(int sensor_id){
    if(MAX_SONARS <= sensor_id){
        return -1;
    }

    return sonar_data[sensor_id];
}
