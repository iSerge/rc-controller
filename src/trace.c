#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>
#include "Drivers/rpi_base.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define AUX_MU_IO_REG   (PRI_BASE_ADDRESS+0x00215040)
#define AUX_MU_LSR_REG  (PRI_BASE_ADDRESS+0x00215054)

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0

//((250,000,000/115200)/8)-1 = 270

void uart_char(unsigned int ra){
    while(1)
        {
            if(GET32(AUX_MU_LSR_REG)&0x20) break;
        }
        PUT32(AUX_MU_IO_REG, ra);
}

void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_char(rc);
        if(rb==0) break;
    }
    uart_char(0x20);
}

void uart_str(const char* str){
    int i;

    if (NULL == str) {
        return;
    }

    for(i=0; 0 != str[i]; ++i) {
        uart_char(str[i]);
    }
}

void uart_strln(const char *str){
    uart_str(str);
    uart_str("\n\r");
}

static char buf[512];

void vAssertCalled(char* file, int line){
    sprintf(buf, "AssertCalled in file: %s\n\r, line: %d\n\r", file, line);
    uart_str(buf);
    for(;;){}
}

void taskSwitchOut(void * taskHandle){
    TaskHandle_t task = (TaskHandle_t)taskHandle;
    TaskStatus_t xTaskDetails;
    //vTaskGetInfo( task, &xTaskDetails, pdTRUE, eInvalid );
    snprintf(buf, 512, "Task <%s> switched out\n\r", pcTaskGetName(task));
    uart_str(buf);

    (void) xTaskDetails;
}

void taskSwitchIn(void * taskHandle){
    TaskHandle_t task = (TaskHandle_t)taskHandle;
    TaskStatus_t xTaskDetails;
    //vTaskGetInfo( task, &xTaskDetails, pdTRUE, eInvalid );
    snprintf(buf, 512, "Task <%s> switched in\n\r", pcTaskGetName(task));
    uart_str(buf);

    (void) xTaskDetails;
}

