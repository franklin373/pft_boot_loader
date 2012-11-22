#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "global.h"

//typedef void (*FUNC_CB_COMM_RCV)(uint8 *buf,int len);

int comm_init(int which_port,int baudrate,FUNC_CB_COMM_RCV cb_rcv)
{
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
#if 0
	uint32_t idx, len;
	__IO FlagStatus exitflag;
	uint8_t buffer[10];
#endif
#if 0
#if (UART_TEST_NUM == 0)
	/*
	 * Initialize UART0 pin connect
	 * P0.2: U0_TXD
	 * P0.3: U0_RXD
	 */
	PINSEL_ConfigPin(0,2,1);
	PINSEL_ConfigPin(0,3,1);
#elif (UART_TEST_NUM == 1)
	/*
	 * Initialize UART1 pin connect
	 * P3.16: U1_TXD
	 * P3.17: U1_RXD
	 */
	PINSEL_ConfigPin(3,16,3);
	PINSEL_ConfigPin(3,17,3);
#elif (UART_TEST_NUM == 2)
	/*
	 * Initialize UART2 pin connect
	 * P0.10: U2_TXD
	 * P0.11: U2_RXD
	 */
	PINSEL_ConfigPin(0,10,1);
	PINSEL_ConfigPin(0,11,1);

#endif
#else
	if (which_port == 0){
		/*
		 * Initialize UART0 pin connect
		 * P0.2: U0_TXD
		 * P0.3: U0_RXD
		 */
		PINSEL_ConfigPin(0,2,1);
		PINSEL_ConfigPin(0,3,1);
	}else if(which_port == 1){
		/*
		 * Initialize UART1 pin connect
		 * P3.16: U1_TXD
		 * P3.17: U1_RXD
		 */
		PINSEL_ConfigPin(3,16,3);
		PINSEL_ConfigPin(3,17,3);
	}else if(which_port == 2){
		/*
		 * Initialize UART2 pin connect
		 * P0.10: U2_TXD
		 * P0.11: U2_RXD
		 */
		PINSEL_ConfigPin(0,10,1);
		PINSEL_ConfigPin(0,11,1);
	}
#endif

	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct,115200);

	// Initialize UART0 peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)uartDrvDataArray[which_port].reg_base, &UARTConfigStruct);


	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Initialize FIFO for UART0 peripheral
	UART_FIFOConfig((LPC_UART_TypeDef *)uartDrvDataArray[which_port].reg_base, &UARTFIFOConfigStruct);


	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)uartDrvDataArray[which_port].reg_base, ENABLE);

    /* Enable UART Rx interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)uartDrvDataArray[which_port].reg_base, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)uartDrvDataArray[which_port].reg_base, UART_INTCFG_RLS, ENABLE);
	/*
	 * Do not enable transmit interrupt here, since it is handled by
	 * UART_Send() function, just to reset Tx Interrupt state for the
	 * first time
	 */
//	TxIntStat = RESET;
#if 0
	// Reset ring buf head and tail idx
	__BUF_RESET(rb.rx_head);
	__BUF_RESET(rb.rx_tail);
	__BUF_RESET(rb.tx_head);
	__BUF_RESET(rb.tx_tail);
#endif
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(uartDrvDataArray[which_port].irqn, ((0x01<<3)|0x01));
	/* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(uartDrvDataArray[which_port].irqn);
	if(cb_rcv){
		uartDrvDataArray[which_port].cb_rcv=cb_rcv;
	}
	return 0;
}

