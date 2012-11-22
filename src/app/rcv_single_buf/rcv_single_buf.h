#ifndef __RCV_SINGLE_BUF_H
#define __RCV_SINGLE_BUF_H

typedef struct
{
    uint8 UartBuf[MAX_PRINT_DATA_LENGTH];
	int uartBufOff;
    uint8 i8UartValidCmd;
} LPC_UARTBlock_t;
extern LPC_UARTBlock_t tUartBlock;
void cb_4_rcv_intr(uint8 *pBuf,int numBytes);

#endif
