#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "global.h"

#if 0
struct tagUartDrvData{
	uint32 reg_base;
	IRQn_Type irqn;
	FUNC_CB_COMM_RCV cb_rcv;
};
#endif
void cb_comm_rcv_null(uint8 *buf,int len)
{
}
struct tagUartDrvData uartDrvDataArray[5]={
	{(uint32)LPC_UART0,UART0_IRQn,cb_comm_rcv_null},
	{(uint32)LPC_UART1,UART1_IRQn,cb_comm_rcv_null},
	{(uint32)LPC_UART2,UART2_IRQn,cb_comm_rcv_null},
	{(uint32)LPC_UART3,UART3_IRQn,cb_comm_rcv_null},
	{(uint32)LPC_UART4,UART4_IRQn,cb_comm_rcv_null},
};
#if 0	
/*in fact, UART register have a bit difference, so usint uint32, not (LPC_UART_TypeDef *)*/
uint32 getUartRegAddrBaseFromUartPort(int which_port)
{
	switch(which_port){
	case 0:
		return (uint32)LPC_UART0;
	case 1:
		return (uint32)LPC_UART1;
	case 2:
		return (uint32)LPC_UART2;
	case 3:
		return (uint32)LPC_UART3;
	case 4:
		return (uint32)LPC_UART4;
	default:
		innerDeadNoOutput();
		return 0;
	}
}
#endif
void UARTPutChar(LPC_UART_TypeDef *UARTx,uint8 ch)
{
#if 0
            U0THR = pBuf[i];
            while( (U0LSR&0x20)==0 );
    LPC_UartWrite(uartPortNr, &ch, 1, 0);
#endif	
		{
			uint32_t timeOut;

			timeOut = UART_BLOCKING_TIMEOUT;

			// Wait for THR empty with timeout
			while (!(UARTx->LSR & UART_LSR_THRE))
			{
				if (timeOut == 0)
					break;

				timeOut--;
			}

			// Time out!
			if(timeOut == 0){
				innerDeadNoOutput();
			}

			UART_SendByte(UARTx, ch);
		}
}
void myfputchar(int uartPortNr,uint8 ch)
{
	LPC_UART_TypeDef *UARTx;

	UARTx=(LPC_UART_TypeDef *)uartDrvDataArray[uartPortNr].reg_base;
	UARTPutChar(UARTx,ch);
}
extern int LPC_UartWrite( int uartPortNr, uint8 *  pBuf,
                          uint32 numBytes)
{
	int i;

	for(i=0;i<numBytes;i++){
		myfputchar(uartPortNr,pBuf[i]);
	}
	return numBytes;
}
extern int LPC_UartPrintf(int uartPortNr, uint8 * pBuf)
{
    return LPC_UartWrite(uartPortNr, (uint8 *)pBuf, strlen((char*)pBuf));
}

