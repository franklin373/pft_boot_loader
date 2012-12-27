#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "global.h"
#include "taskfrag/taskfrag.h"
#include "drv_gpio.h"

#define LED_PARA_COMPARE_TO_P25 (479)
void led_FSA()
{
    static uint32 	uiLedCount = 0;
    static uint32 	uiLedStatus=0;

    uiLedCount++;
    if(uiLedCount==5000/LED_PARA_COMPARE_TO_P25 || uiLedCount==80000/LED_PARA_COMPARE_TO_P25)
    {
    	if(uiLedStatus)
    	{
    		uiLedStatus = 0;
	    	gbLedOff();
	    }
	    else
	    {
	    	uiLedCount = 0;
	    	uiLedStatus = 1;
	    	gbLedOn();
	    }
    }
}
int tf_per_tick_run_count_for_debug;
void tf_per_tick()
{
	++tf_per_tick_run_count_for_debug;
	led_FSA();
}
