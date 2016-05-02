/*
 * rpio_gpio.c
 *
 *  Created on: Mar 7, 2016
 *      Author: TsungMin Huang
 */

#include <stdint.h>

#include "rpi_gpio.h"
#include "rpi_irq.h"

void rpi_gpio_sel_fun(uint32_t pin, uint32_t func) {
    int offset = pin / 10;
    uint32_t val = RPI_GPIO->GPFSEL[offset];// Read in the original register value.

    int item = pin % 10;
    val &= ~(0x7 << (item * 3));
    val |= ((func & 0x7) << (item * 3));
    RPI_GPIO->GPFSEL[offset] = val;
}

void rpi_gpio_set_val(uint32_t pin, uint32_t val) {
    uint32_t offset = pin / 32;
    uint32_t mask = (1 << (pin % 32));

    if (val) {
        RPI_GPIO->GPSET[offset] |= mask;
    } else {
        RPI_GPIO->GPCLR[offset] |= mask;
    }
}

uint32_t rpi_gpio_get_val(uint32_t pin) {
    uint32_t offset = pin / 32;
    uint32_t mask = (1 <<(pin % 32));
    uint32_t result = RPI_GPIO->GPLEV[offset] & mask;

    return result ? 1 : 0;
}

void rpi_gpio_toggle(uint32_t pin) {

    uint32_t result = rpi_gpio_get_val(pin);
    if(result) {
        rpi_gpio_set_val(pin, 0);
    } else {
        rpi_gpio_set_val(pin, 1);
    }
}

uint32_t rpi_gpio_ev_read_status(uint32_t pin, GPIO_EV_SEL_t event){
    uint32_t offset = pin / 32;
    uint32_t mask = (1 <<(pin % 32));
    uint32_t result = 0;

    if(event & GPIO_EV_STATUS){
        result = RPI_GPIO->GPEDS[offset];
    }

    if(event & GPIO_EV_RISING_EDGE){
        result = RPI_GPIO->GPREN[offset];
    }

    if(event & GPIO_EV_FALLING_EDGE){
        result = RPI_GPIO->GPFEN[offset];
    }

    if(event & GPIO_EV_HIGH_LVL){
        result = RPI_GPIO->GPHEN[offset];
    }

    if(event & GPIO_EV_LOW_LVL){
        result = RPI_GPIO->GPLEN[offset];
    }

    if(event & GPIO_EV_ASYNC_RISING_EDGE){
        result = RPI_GPIO->GPAREN[offset];
    }

    if(event & GPIO_EV_ASYNC_FALLING_EDGE){
        result = RPI_GPIO->GPAFEN[offset];
    }

    return (result & mask) ? 1 : 0;
}

void rpi_gpio_ev_clear_status(uint32_t pin){
    uint32_t offset = pin / 32;
    uint32_t mask = (1 <<(pin % 32));

    RPI_GPIO->GPEDS[offset] = mask;
}

void rpi_gpio_ev_detect_enable(uint32_t pin, GPIO_EV_SEL_t events){
    uint32_t offset = pin / 32;
    uint32_t mask = (1 <<(pin % 32));

    if(events & GPIO_EV_STATUS){
        RPI_GPIO->GPEDS[offset] = mask;
    }

    if(events & GPIO_EV_RISING_EDGE){
        RPI_GPIO->GPREN[offset] |= mask;
    }

    if(events & GPIO_EV_FALLING_EDGE){
        RPI_GPIO->GPFEN[offset] |= mask;
    }

    if(events & GPIO_EV_HIGH_LVL){
        RPI_GPIO->GPHEN[offset] |= mask;
    }

    if(events & GPIO_EV_LOW_LVL){
        RPI_GPIO->GPLEN[offset] |= mask;
    }

    if(events & GPIO_EV_ASYNC_RISING_EDGE){
        RPI_GPIO->GPAREN[offset] |= mask;
    }

    if(events & GPIO_EV_ASYNC_FALLING_EDGE){
        RPI_GPIO->GPAFEN[offset] |= mask;
    }
}

void rpi_gpio_ev_detect_disable(uint32_t pin, GPIO_EV_SEL_t events){
    uint32_t offset = pin / 32;
    uint32_t mask = (1 <<(pin % 32));

    if(events & GPIO_EV_STATUS){
        RPI_GPIO->GPEDS[offset] = mask;
    }

    if(events & GPIO_EV_RISING_EDGE){
        RPI_GPIO->GPREN[offset] &= ~mask;
    }

    if(events & GPIO_EV_FALLING_EDGE){
        RPI_GPIO->GPFEN[offset] &= ~mask;
    }

    if(events & GPIO_EV_HIGH_LVL){
        RPI_GPIO->GPHEN[offset] &= ~mask;
    }

    if(events & GPIO_EV_LOW_LVL){
        RPI_GPIO->GPLEN[offset] &= ~mask;
    }

    if(events & GPIO_EV_ASYNC_RISING_EDGE){
        RPI_GPIO->GPAREN[offset] &= ~mask;
    }

    if(events & GPIO_EV_ASYNC_FALLING_EDGE){
        RPI_GPIO->GPAFEN[offset] &= ~mask;
    }
}

#define RPI_TOTAL_GPIO 54
static RPI_GPIO_EV_TABLE_t g_rpi_gpio_ev_table[RPI_TOTAL_GPIO];

static uint32_t clz(uint32_t event_detect_status) {
    uint32_t pin = 0;
    uint32_t mask = 1UL;
    while(!(event_detect_status & mask)){
        ++pin;
        mask <<= 1;
    }

    return pin;
}

static void rpi_gpio_ev_dispatcher(void *pParam){
    uint32_t event_detect_status;
    uint32_t pin;

    event_detect_status = RPI_GPIO->GPEDS[0];
    if(event_detect_status){
        pin = clz(event_detect_status);
        goto call_handler;
    }

    event_detect_status = RPI_GPIO->GPEDS[1] & 0x3FFFFFUL;
    if (event_detect_status){
        pin = clz(event_detect_status) + 32;
        goto call_handler;
    }

    return;

 call_handler:
    if((void*)0 != g_rpi_gpio_ev_table[pin].pHandler){
        g_rpi_gpio_ev_table[pin].pHandler(g_rpi_gpio_ev_table[pin].pParam);
    } else {
        rpi_gpio_ev_clear_status(pin);
    }

    (void)pParam;
}

void rpi_gpio_init_ev_facility(){
    int i;
    for(i = 0; i < RPI_TOTAL_GPIO; ++i){
        g_rpi_gpio_ev_table[i].pHandler = (void*) 0;
        g_rpi_gpio_ev_table[i].pParam = (void*) 0;
    }

    //Disable all GPIO events
    RPI_GPIO->GPREN[0] = 0;
    RPI_GPIO->GPREN[1] = 0;

    RPI_GPIO->GPFEN[0] = 0;
    RPI_GPIO->GPFEN[1] = 0;

    RPI_GPIO->GPHEN[0] = 0;
    RPI_GPIO->GPHEN[1] = 0;

    RPI_GPIO->GPLEN[0] = 0;
    RPI_GPIO->GPLEN[1] = 0;

    RPI_GPIO->GPAREN[0] = 0;
    RPI_GPIO->GPAREN[1] = 0;

    RPI_GPIO->GPAFEN[0] = 0;
    RPI_GPIO->GPAFEN[1] = 0;

    RPI_GPIO->GPEDS[0] = 0xFFFFFFFF;
    RPI_GPIO->GPEDS[1] = 0x003FFFFF;

    rpi_irq_register_handler(RPI_IRQ_ID_GPIO_0, rpi_gpio_ev_dispatcher, (void*)0);
    rpi_irq_register_handler(RPI_IRQ_ID_GPIO_1, rpi_gpio_ev_dispatcher, (void*)1);
    rpi_irq_register_handler(RPI_IRQ_ID_GPIO_2, rpi_gpio_ev_dispatcher, (void*)2);
    //rpi_irq_register_handler(RPI_IRQ_ID_GPIO_3, rpi_gpio_ev_dispatcher, (void*)3);
}

int rpi_gpio_register_ev_handler(uint32_t pin,
                                  RPI_GPIO_EV_HANDLER_t pHandler,
                                  void *pParam)
{
    if(RPI_TOTAL_GPIO <= pin){
        return -1;
    }
    
    g_rpi_gpio_ev_table[pin].pHandler = pHandler;
    g_rpi_gpio_ev_table[pin].pParam = pParam;

    return pin;
}
