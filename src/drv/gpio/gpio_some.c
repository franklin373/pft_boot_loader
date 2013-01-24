#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_gpio.h"
#include "global.h"
#include "taskfrag/taskfrag.h"
#include "rcv_single_buf/rcv_single_buf.h"
#include "flash/norflash_sst39vf6401b.h"
#include "flash/flash_common.h"
#include "drv_gpio.h"

void led_init(void)
{
    // 2.3 
    // 2.4 
    // 0.27 
    // 2.6
    
    GPIO_SetDir(4, (1<<22), 1);
    GPIO_SetDir(4, (1<<23), 1);
    GPIO_SetDir(4, (1<<26), 1);
    GPIO_SetDir(4, (1<<27), 1);
}
void gbLedOn()
{
	GPIO_SetValue(4, (1<<22));
}
void gbLedOff()
{
	GPIO_ClearValue(4, (1<<22));
}
void KeyInit(void)
{
    GPIO_SetDir(5, (1<<0), 0);
    GPIO_SetDir(5, (1<<1), 0);
    GPIO_SetDir(5, (1<<2), 0);
    GPIO_SetDir(5, (1<<3), 0);
}
int TestFeedKey()
{
	return (GPIO_ReadValue(5)&(1<<1))?0:1;
}
void gpio_some_init()
{
	led_init();
	KeyInit();
}

