//#include "config.h"
#include "global.h"
//#include  "lpc2210.h"
//#include "halPrn.h"

void innerDeadNoOutput()
{
	int i;
	
	//TODO, set LED
        for(i=0;i<6000;i++);
//        post_mordem_process();
        for(;;);
//        *(uint32 *)70000000=*(uint32 *)70000000;//this would cause core dump, but not work
}
void assert_mine(char *filename,int lineno,int value)
{
	if(!value){
//        OS_CPU_SR  cpu_sr = 0;
        static char tmp[500];
        volatile register int i;
                
//        cpu_sr = OS_CPU_SR_Save();
        sprintf(tmp,"assert_mine(%s:%d,%d)\r\n",filename,lineno,value);
        LPC_UartPrintf(PRN_DEV_LOG,(uint8 *)tmp);
//        LPC_UartPrintf(1,(uint8 *)tmp);
		innerDeadNoOutput();
	}
}
