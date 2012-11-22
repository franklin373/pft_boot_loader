#include "global.h"
//#include "fifo/serlDrvInner.h"
#include "rcv_single_buf.h"
#include "taskfrag/taskfrag.h"
//#include "uartInt.h"
#include "protocol/P25_protocol.h"

LPC_UARTBlock_t tUartBlock;
void cb_4_rcv_intr_byte(uint8 RevByte)
{
	if(tUartBlock.uartBufOff>=MAX_PRINT_DATA_LENGTH-1){
		innerDeadNoOutput();
	}
	switch(RevByte){
	case PRN_FRAME_SOF:
		tUartBlock.uartBufOff=0;
		tUartBlock.UartBuf[tUartBlock.uartBufOff++]=RevByte;
		break;
	case PRN_FRAME_EOF:
		tUartBlock.UartBuf[tUartBlock.uartBufOff++]=RevByte;
		tUartBlock.i8UartValidCmd=1;
		TF_schedule(0,0);
		break;
	default:
		if(!tUartBlock.i8UartValidCmd){
			tUartBlock.UartBuf[tUartBlock.uartBufOff++]=RevByte;
		}
		break;
	}
}
void cb_4_rcv_intr(uint8 *pBuf,int numBytes)
{
	int i;
	
	for(i=0;i<numBytes;i++){
		cb_4_rcv_intr_byte(pBuf[i]);
	}
}
