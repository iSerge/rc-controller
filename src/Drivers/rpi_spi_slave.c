#include "rpi_irq.h"
#include "rpi_gpio.h"
#include "rpi_spi_slave.h"

#include "trace.h"

#define BUF_LEN 32

static uint8_t buf[BUF_LEN];
static uint32_t buf_start = 0;
static uint32_t buf_end = 0;

static uint32_t irq_count = 0;

void rpi_slave_irq_handler(int nIRQ, void* pParam)
{
    uint32_t int_status = BSC_SL->MIS;
    uint32_t flags = BSC_SL->FR;
    static uint32_t data;
    
    //BSC_SL->ICR = 0x0FUL;
    //uart_strln("SPI slave IRQ");

    ++irq_count;
    if(int_status & BSC_SL_MIS_RXMIS){
        while( 0 != (flags & BSC_SL_FR_RXLVL_MASK) >> BSC_SL_FR_RXLVL_SHIFT){
            data = BSC_SL->DR & 0xFF;
            buf[buf_start++] = (uint8_t)data;
            BSC_SL->DR = data; //send received data back
            if(buf_start >= BUF_LEN) buf_start = 0;
            flags = BSC_SL->FR;
        }
    }

    if(int_status & BSC_SL_MIS_TXMIS){
        // Move data from buffer to FIFO
    }

    (void)int_status;
    (void)nIRQ;
    (void)pParam;
}

static void init_bsc_spi_slave(){
    rpi_gpio_sel_fun(18, GPIO_FSEL_ALT3);
    rpi_gpio_sel_fun(19, GPIO_FSEL_ALT3);
    rpi_gpio_sel_fun(20, GPIO_FSEL_ALT3);
    rpi_gpio_sel_fun(21, GPIO_FSEL_ALT3);

    rpi_irq_register_handler(RPI_IRQ_ID_SPI_SLAVE, rpi_slave_irq_handler, 0);
    rpi_irq_enable(RPI_IRQ_ID_SPI_SLAVE);

    BSC_SL->CR = BSC_SL_CR_BRK;
    BSC_SL->FR = 0;
    BSC_SL->RSR = 0;
    BSC_SL->IFLS = BSC_SL_IFLS_RXIFLSEL_1_2 | BSC_SL_IFLS_TXIFLSEL_1_4;
    BSC_SL->IMSC = BSC_SL_IMSC_RXIM;// | BSC_SL_IMSC_TXIM;
    BSC_SL->SLV = 0x08UL;
    BSC_SL->CR = BSC_SL_CR_RXE | BSC_SL_CR_TXE | BSC_SL_CR_SPI_MODE_0 |
        BSC_SL_CR_EN;
}

//static void driver_task(void *pParam){
    // Suppusedly will be used for spi message dispatching
//}

void bsc_spi_slave_init()
{
    int32_t i;
    
    for(i = 0; i < BUF_LEN; ++i){
        buf[i] = i & 0xFF;
    }

    init_bsc_spi_slave();
}

void rpi_spi_slave_init(){
    bsc_spi_slave_init();
}
        


const uint8_t* get_spi_buffer(void)
{
    return buf;
}

uint32_t read_spi_buf(uint32_t buf_len, uint8_t* out_buf)
{
    uint32_t cnt = 0;
    uint32_t i = buf_start;

    while(i != buf_end && cnt < buf_len){
        out_buf[cnt++] = buf[i++];
        if(BUF_LEN == i) i = 0;
    }

    return cnt;
} 

uint32_t spi_slave_irq(){
    return irq_count;
}
