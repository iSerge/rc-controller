#ifndef __RPI_SPI_SLAVE_H__
#define __RPI_SPI_SLAVE_H__

#include "rpi_base.h"

#define BSC_SLAVE_ADDR (PRI_BASE_ADDRESS + 0x214000)

typedef struct {
    volatile uint32_t DR;
    volatile uint32_t RSR;
    volatile uint32_t SLV;
    volatile uint32_t CR;
    volatile uint32_t FR;
    volatile uint32_t IFLS;
    volatile uint32_t IMSC;
    volatile uint32_t RIS;
    volatile uint32_t MIS;
    volatile uint32_t ICR;
    volatile uint32_t DMACR;
    volatile uint32_t TDR;
    volatile uint32_t GPUSTAT;
    volatile uint32_t HCTRL;
    volatile uint32_t DEBUG1;
    volatile uint32_t DEBUG2;
} __attribute__((packed, aligned(1))) BSC_SLAVE_t;
    
#define BSC_SL ((BSC_SLAVE_t*)BSC_SLAVE_ADDR)

#define BSC_SL_DR_OE 0x0100
#define BSC_SL_DR_UE 0x0200

#define BSC_SL_DR_TXBUSY 0x010000
#define BSC_SL_DR_RXFE   0x020000
#define BSC_SL_DR_TXFF   0x040000
#define BSC_SL_DR_RXFF   0x080000
#define BSC_SL_DR_TXFE   0x100000
#define BSC_SL_DR_RXBUSY 0x200000

#define BSC_SL_DR_TXLVL_MASK  0xF8000000
#define BSC_SL_DR_TXLVL_SHIFT 22
#define BSC_SL_DR_RXLVL_MASK  0x07C00000
#define BSC_SL_DR_RXLVL_SHIFT 27

#define BSC_SL_RSR_OE 0x01
#define BSC_SL_RSR_UE 0x02

#define BSC_SL_CR_EN         0x0001
#define BSC_SL_CR_SPI        0x0002
#define BSC_SL_CR_I2C        0x0004
#define BSC_SL_CR_CPHA_0     0x0000
#define BSC_SL_CR_CPHA_1     0x0008
#define BSC_SL_CR_CPOL_0     0x0000
#define BSC_SL_CR_CPOL_1     0x0010
#define BSC_SL_CR_ENSTAT     0x0020
#define BSC_SL_CR_ENCTRL     0x0040
#define BSC_SL_CR_BRK        0x0080
#define BSC_SL_CR_TXE        0x0100
#define BSC_SL_CR_RXE        0x0200
#define BSC_SL_CR_INVRXF     0x0400
#define BSC_SL_CR_TESTFIFO   0x0800
#define BSC_SL_CR_HISTCTRLEN 0x1000
#define BSC_SL_CR_INV_TXF    0x2000

#define BSC_SL_FR_TXBUSY 0x01
#define BSC_SL_FR_RXFE   0x02
#define BSC_SL_FR_TXFF   0x04
#define BSC_SL_FR_RXFF   0x08
#define BSC_SL_FR_TXFE   0x10
#define BSC_SL_FR_RXBUSY 0x20

#define BSC_SL_FR_TXLVL_MASK  0xF800
#define BSC_SL_FR_TXLVL_SHIFT 6
#define BSC_SL_FR_RXLVL_MASK  0x07C0
#define BSC_SL_FR_RXLVL_SHIFT 11

#define BSC_SL_IFLS_TXIFLSEL_1_8 0
#define BSC_SL_IFLS_TXIFLSEL_1_4 1
#define BSC_SL_IFLS_TXIFLSEL_1_2 2
#define BSC_SL_IFLS_TXIFLSEL_3_4 3
#define BSC_SL_IFLS_TXIFLSEL_7_8 4

#define BSC_SL_IFLS_RXIFLSEL_1_8 0
#define BSC_SL_IFLS_RXIFLSEL_1_4 8
#define BSC_SL_IFLS_RXIFLSEL_1_2 16
#define BSC_SL_IFLS_RXIFLSEL_3_4 24
#define BSC_SL_IFLS_RXIFLSEL_7_8 32

#define BSC_SL_IMSC_RXIM 0x01
#define BSC_SL_IMSC_TXIM 0x02
#define BSC_SL_IMSC_BEIM 0x04
#define BSC_SL_IMSC_OEIM 0x08

#define BSC_SL_RIS_RXRIS 0x01
#define BSC_SL_RIS_TXRIS 0x02
#define BSC_SL_RIS_BERIS 0x04
#define BSC_SL_RIS_OERIS 0x08

#define BSC_SL_MIS_RXMIS 0x01
#define BSC_SL_MIS_TXMIS 0x02
#define BSC_SL_MIS_BEMIS 0x04
#define BSC_SL_MIS_OEMIS 0x08

#define BSC_SL_ICR_RXRIC 0x01
#define BSC_SL_ICR_TXRIC 0x02
#define BSC_SL_ICR_BERIC 0x04
#define BSC_SL_ICR_OERIC 0x08


void rpi_spi_slave_init();

#endif
