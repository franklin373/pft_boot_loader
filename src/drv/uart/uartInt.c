#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "global.h"
#include "drv_uart_inner.h"

/*********************************************************************//**
 * @brief		UART Line Status Error
 * @param[in]	bLSErrType	UART Line Status Error Type
 * @return		None
 **********************************************************************/
void UART_IntErr(uint8_t bLSErrType)
{
	uint8_t test;
	// Loop forever
	while (1){
		// For testing purpose
		test = bLSErrType;
	}
}
void UART_IntReceive(int which_port)
{
	uint8_t tmpc;
	uint32_t rLen;
	LPC_UART_TypeDef *UARTx;

	UARTx=(LPC_UART_TypeDef *)uartDrvDataArray[which_port].reg_base;
	while(1){
		// Call UART read function in UART driver
		rLen = UART_Receive(UARTx, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){
#if 0			
			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
			if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
				rb.rx[rb.rx_head] = tmpc;
				__BUF_INCR(rb.rx_head);
			}
#else
			uartDrvDataArray[which_port].cb_rcv(&tmpc,rLen);
#endif
		}
		// no more data
		else {
			break;
		}
	}
}
void Uart_X_Isr(int which_port)
{
	uint32_t intsrc, tmp, tmp1;
	LPC_UART_TypeDef *UARTx;

	UARTx=(LPC_UART_TypeDef *)uartDrvDataArray[which_port].reg_base;
	/* Determine the interrupt source */
	intsrc = UART_GetIntId(UARTx);
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = UART_GetLineStatus(UARTx);
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {
				UART_IntErr(tmp1);
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
			UART_IntReceive(which_port);
	}

	// Transmit Holding Empty
	if (tmp == UART_IIR_INTID_THRE){
#if 0		
			UART_IntTransmit();
#else
			innerDeadNoOutput();
#endif
	}

}
void UART0_IRQHandler(void)
{
	Uart_X_Isr(0);
}
void UART2_IRQHandler(void)
{
	Uart_X_Isr(2);
}

