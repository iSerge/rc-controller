#include "rpi_gpio.h"
#include "rpi_spi_slave.h"

void rpi_spi_slave_init(){
    rpi_gpio_sel_fun(18, GPIO_FSEL_ALT3);
    rpi_gpio_sel_fun(19, GPIO_FSEL_ALT3);
    rpi_gpio_sel_fun(20, GPIO_FSEL_ALT3);
    rpi_gpio_sel_fun(21, GPIO_FSEL_ALT3);
}

