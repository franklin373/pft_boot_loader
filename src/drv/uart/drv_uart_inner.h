#ifndef __DRV_UART_INNER_H
#define __DRV_UART_INNER_H

struct tagUartDrvData{
	uint32 reg_base;
	IRQn_Type irqn;
	FUNC_CB_COMM_RCV cb_rcv;
};
extern struct tagUartDrvData uartDrvDataArray[];

#endif

