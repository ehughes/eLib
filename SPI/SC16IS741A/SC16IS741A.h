#include "Components/Queue/eQueue.h"

#ifndef _SC16IS741A_H
#define _SC16IS741A_H

void SC16IS741A_Init();

void SC16IS741A_Process();

#define SC16IS741A__READ_REG    0x80

#define SC16IS741A__CH0         (1<<1)
#define SC16IS741A__CH1         (1<<2)


#define SC16IS741A__RHR         0x00
#define SC16IS741A__THR         0x00
#define SC16IS741A__IER         0x01
#define SC16IS741A__FCR         0x02
#define SC16IS741A__IIR         0x02
#define SC16IS741A__LCR         0x03
#define SC16IS741A__MCR         0x04
#define SC16IS741A__LSR         0x05
#define SC16IS741A__MSR         0x06
#define SC16IS741A__SPR         0x07
#define SC16IS741A__TCR         0x06 // //only when MCR[2] == 1 and EFR[4] == 1
#define SC16IS741A__TLR         0x07
#define SC16IS741A__TXLVL       0x08
#define SC16IS741A__RXLVL       0x09
#define SC16IS741A__RSV         0x0D 
#define SC16IS741A__UART_RST    0x0E
#define SC16IS741A__EFCR        0x0F

#define SC16IS741A__SPECIAL_DLL     0x00
#define SC16IS741A__SPECIAL_DLH     0x01
#define SC16IS741A__SPECIAL_XON1    0x02
#define SC16IS741A__SPECIAL_XON2    0x04
#define SC16IS741A__SPECIAL_XOFF1   0x05
#define SC16IS741A__SPECIAL_XOFF2   0x06

#define SC16IS741A__LCR_BIT__DIVISOR_LATCH_ENABLE   (1<<7)

#define SC16IS741A__LCR_BITS__WORD_LENGTH_8BIT       (0x3)

#define SC16IS741A__FCR_BITS__FIFO_ENABLE            (1<<0)

extern ByteQueue SPI_UART_TX_Q;


void SC16IS741A_WriteRegister(uint8_t Register, uint8_t Value);
void SC16IS741A_WriteTxFIFO(uint8_t *Value, uint32_t Length);
void SC16IS741A_ReadRxFIFO(uint8_t *Value, uint32_t Length);
uint8_t SC16IS741A_ReadRegister(uint8_t Register);

#endif