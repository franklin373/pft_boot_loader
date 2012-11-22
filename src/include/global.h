#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "lpc_types.h"
#include "LPC177x_8x.h"
#include "debug_frmwrk.h"
//This segment should not be modified



typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef /*signed*/   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */



typedef uint8 BYTE;
typedef uint32 DWORD;
typedef unsigned int UINT;
typedef int HANDLE;
typedef int BOOL;




//#define     USER_USING_MODE    0x10             /*  User mode ,ARM 32BITS CODE 用户模式,ARM代码                  */

                                                     /*  Chosen one from 0x10,0x30,0x1f,0x3f.只能是0x10,0x30,0x1f,0x3f之一       */
typedef void (*pHandler)(void*);

#define HANDLER_NULL   (pHandler)0

#define MAX_PRINT_DATA_LENGTH 1024*10

#define  Uart0BufLen  MAX_PRINT_DATA_LENGTH
#define  Uart1BufLen  MAX_PRINT_DATA_LENGTH

//uart assign
#define IrDAPort  LPC_UART_0
#define StdioPort LPC_UART_1

/************************************/
/*Application Program Configurations*/
/************************************/
//This segment could be modified as needed.
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
//#include    <rt_misc.h>






typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef  CPU_INT32U                 CPU_SR;                     /* Defines   CPU status register size (see Note #3b).   */
typedef unsigned int   OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */
#define  CPU_SR_ALLOC()             CPU_SR  cpu_sr = (CPU_SR)0;
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}
OS_CPU_SR  OS_CPU_SR_Save(void);
void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);




extern volatile  uint32  OSTime;                   /* Current value of system time (in ticks)         */




#if 0
/********************************/
/********Configuration***********/
/********************************/
/* System configuration .Fosc、Fcclk、Fcco、Fpclk must be defined */


#define Fosc            11059200                    //Crystal frequence,10MHz~25MHz，should be the same as actual status.

#define Fcclk           (Fosc * 4)                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz.

#define Fcco            (Fcclk * 4)                 //CCO frequence,should be 2、4、8、16 multiples of Fcclk, ranged from 156MHz to 320MHz.

#define Fpclk           (Fcclk / 4) * 4             //VPB clock frequence , must be 1、2、4 multiples of (Fcclk / 4).
#endif

void assert_mine(char *filename,int lineno,int value);
#define ASSERT_MINE(value) do{assert_mine(__FILE__,__LINE__,value);}while(0)

#define TICK_PERIOD (10)
#define  ARR_SIZE( a )  ( sizeof( (a) ) / sizeof( (a[0]) ) )

typedef enum
{
    PRN_DEV_WIRE,
    PRN_DEV_NOUSE,
	PRN_DEV_LOG,
} LPC_UARTDevice;



//uint32 getUartRegAddrBaseFromUartPort(int which_port);
typedef void (*FUNC_CB_COMM_RCV)(uint8 *buf,int len);
struct tagUartDrvData{
	uint32 reg_base;
	IRQn_Type irqn;
	FUNC_CB_COMM_RCV cb_rcv;
};
extern struct tagUartDrvData uartDrvDataArray[];
void innerDeadNoOutput();
extern int LPC_UartWrite( int uartPortNr, uint8 *  pBuf,uint32 numBytes);
extern int LPC_UartPrintf(int uartPortNr, uint8 * pBuf);
int comm_init(int which_port,int baudrate,FUNC_CB_COMM_RCV cb_rcv);
void myfputchar(int uartPortNr,uint8 ch);
void tf_rcv_single_buf();
void tick_init();

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

