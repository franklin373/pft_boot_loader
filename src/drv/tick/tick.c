#include "global.h"
#include "system_lpc177x_8x.h"
#include "lpc177x_8x_systick.h"

void tick_init()
{
    //SystemInit();   // init system core clock
    SystemCoreClockUpdate();// get SystemCoreClock
    SYSTICK_InternalInit(/*(uint32_t)(1000.0F / (float)OS_TICKS_PER_SEC)*/TICK_PERIOD); // init os tick timer
    SYSTICK_IntCmd(ENABLE);	// enable os tick timer interrupt
    SYSTICK_Cmd(ENABLE);    // enable os tick timer counter
}
volatile  uint32  OSTime;                   /* Current value of system time (in ticks)         */
void SysTick_Handler(void)
{
    CPU_SR_ALLOC();

    OS_ENTER_CRITICAL();                                   /* Update the 32-bit tick counter               */
    OSTime++;
    OS_EXIT_CRITICAL();
}
