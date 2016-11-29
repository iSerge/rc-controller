#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "Drivers/rpi_irq.h"
#include "Drivers/rpi_gpio.h"
#include "Drivers/rpi_i2c.h"
#include "Drivers/accel_driver.h"
#include "Drivers/hc_sr04_driver.h"
#include "Drivers/rpi_bsc_slave.h"

extern void uart_strln(const char *str);

static SemaphoreHandle_t xMutex = NULL;
static const TickType_t xDelay = pdMS_TO_TICKS(500);


void task1(void *pParam) {
    xMutex = xSemaphoreCreateMutex();
    
	while(1) {
        if(pdTRUE == xSemaphoreTake(xMutex, 10)){
            rpi_gpio_set_val(47, 1);
            rpi_gpio_set_val(35, 0);
            vTaskDelay(xDelay);
            xSemaphoreGive(xMutex);
            vTaskDelay(xDelay);
        }
	}
    (void) pParam;
}

void task2(void *pParam) {
    while(NULL == xMutex){}

	while(1) {
        if(pdTRUE == xSemaphoreTake(xMutex, 10)){
            rpi_gpio_set_val(47, 0);
            rpi_gpio_set_val(35, 1);
            vTaskDelay(xDelay);
            xSemaphoreGive(xMutex);
            vTaskDelay(xDelay);
        }
	}
    (void) pParam;
}


/**
 *	This is the systems main entry, some call it a boot thread.
 *
 *	-- Absolutely nothing wrong with this being called main(), just it doesn't have
 *	-- the same prototype as you'd see in a linux program.
 **/
extern void status_task(void *pParam);

int main(void) {
    BaseType_t xReturned;
    
	rpi_cpu_irq_disable();
	rpi_irq_init();

    rpi_gpio_init_ev_facility();

	rpi_gpio_sel_fun(47, GPIO_FSEL_OUT);			// RDY led
	rpi_gpio_sel_fun(35, GPIO_FSEL_OUT);			// RDY led

    rpi_i2c_init();
    init_accel_driver();
    uart_strln("Main: created accel driver task");
    init_sonar_driver();
    uart_strln("Main: created sonar driver task");

    rpi_spi_slave_init();

    xReturned = xTaskCreate(status_task, "Status", 512, NULL, 2, NULL);
    if (pdPASS == xReturned){
        uart_strln("Main: created status task");
    } else {
        uart_strln("Main: status task creation fail");
    }

	xReturned = xTaskCreate(task1, "LED_0", 128, NULL, 1, NULL);
    if (pdPASS == xReturned){
        uart_strln("Main: created task1");
    } else {
        uart_strln("Main: task1 creation fail");
    }

	xReturned = xTaskCreate(task2, "LED_1", 128, NULL, 1, NULL);
    if (pdPASS == xReturned){
        uart_strln("Main: created task2");
    } else {
        uart_strln("Main: task2 creation fail");
    }

    //init_accel_driver();
    //uart_strln("Main: started driver");

    rpi_cpu_irq_enable();
    uart_strln("Main: enabled irqs");

	vTaskStartScheduler();

	/*
	 *	We should never get here, but just in case something goes wrong,
	 *	we'll place the CPU into a safe loop.
	 */
	while(1) {
		;
	}

    return 0;
}

