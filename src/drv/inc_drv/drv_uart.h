#ifndef __DRV_UART_H
#define __DRV_UART_H
#include "debug_frmwrk.h"

#if 0
typedef enum
{
    PRN_DEV_WIRE,
    PRN_DEV_NOUSE,
	PRN_DEV_LOG,
} LPC_UARTDevice;
#else
#define PRN_DEV_WIRE 0
#define PRN_DEV_LOG 2
#endif

#if (PRN_DEV_LOG != USED_UART_DEBUG_PORT)
	#error "PRN_DEV_LOG not equel to USED_UART_DEBUG_PORT"
#endif



//uint32 getUartRegAddrBaseFromUartPort(int which_port);
typedef void (*FUNC_CB_COMM_RCV)(uint8 *buf,int len);
extern int LPC_UartWrite( int uartPortNr, uint8 *  pBuf,uint32 numBytes);
extern int LPC_UartPrintf(int uartPortNr, uint8 * pBuf);
int comm_init(int which_port,int baudrate,FUNC_CB_COMM_RCV cb_rcv);
void myfputchar(int uartPortNr,uint8 ch);

#endif
