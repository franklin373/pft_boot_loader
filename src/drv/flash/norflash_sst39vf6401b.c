/***********************************************************************//**
 * @file		norflash_sst39vf6401b.c
 * @brief		Contains all functions support for NOR Flash SST39VF6401B
 *				
 * @version		1.0
 * @date		2011.8.5
 * @author		Tenghua SW Application Team
 **************************************************************************
 **********************************************************************/
#include "norflash_sst39vf6401b.h"
#include "lpc177x_8x_emc.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"

int32_t volatile timerdev = 0;

/*********************************************************************//**
 * @brief 		Delay
 * @param[in]	delayCnt Delay value
 * @return 		None
 **********************************************************************/
void delay(uint32_t delayCnt)
{
	uint32_t i;

	for ( i = 0; i < delayCnt; i++ );
	return;
}

/*********************************************************************//**
 * @brief 		Initialize external NOR FLASH memory
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void NORFLASHInit( void )
{
	TIM_TIMERCFG_Type TIM_ConfigStruct;
    uint8_t i;
	/**************************************************************************
	* Initialize EMC for NOR FLASH
	**************************************************************************/
	//EMC_Init();
   
	// Enable clock for EMC 
	LPC_SC->PCONP |= 0x00000800;        //PCEMC=1
	LPC_SC->EMCDLYCTL = 0x00001010;
	LPC_EMC->Control = 0x00000001;      //EMC Enable
	LPC_EMC->Config = 0x00000000;       //Little-endian mode

  	/* Pin configuration:
	* P4.24 - /EMC_OE
	* P4.25 - /EMC_WE
   	* P4.30 - /EMC_CS0
   	*
   	* P3.0-P3.15 - EMC_D[0-15]
   	* P4.0-P4.21 - EMC_A[0-21]
   	*/
	for(i = 0; i < 16; i++)
	{
		PINSEL_ConfigPin(3,i,1);	//pin3.0~pin3.15 as EMC_D0~EMC_D15
	}
	for(i = 0; i < 22; i++)
	{
		PINSEL_ConfigPin(4,i,1);	//pin4.0~pin4.21 as EMC_A0~EMC_A21
	}
    PINSEL_ConfigPin(4,24,1);	    //pin4.24 as EMC_OE
	PINSEL_ConfigPin(4,25,1);	    //pin4.25 as EMC_WE
	PINSEL_ConfigPin(4,30,1);	    //pin4.30 as EMC_CS0
    //============================================================

    // init timer
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_Waitms(20);
    
	//Configure the memory width:CS0,Memory width 16bit
	EMC_StaMemConfigMW (0,EMC_StaticConfig_MW_16BITS);
	//Configure the byte lane state:CS0,reads all bits in BLSn[3:0] are LOW
	EMC_StaMemConfigPB(0,EMC_StaticConfig_PB);
	EMC_SetStaMemoryParameter(0, EMC_STA_MEM_WAITWEN, EMC_StaticWaitWen_WAITWEN(2));
	EMC_SetStaMemoryParameter(0, EMC_STA_MEM_WAITOEN, EMC_StaticWaitOen_WAITOEN(2));	
	EMC_SetStaMemoryParameter(0, EMC_STA_MEM_WAITRD, EMC_StaticWaitRd_WAITRD(0x1f));
	EMC_SetStaMemoryParameter(0, EMC_STA_MEM_WAITPAGE, EMC_StaticwaitPage_WAITPAGE(0x1f));
	EMC_SetStaMemoryParameter(0, EMC_STA_MEM_WAITWR, EMC_StaticWaitwr_WAITWR(0x1f));
	EMC_SetStaMemoryParameter(0, EMC_STA_MEM_WAITTURN, EMC_StaticWaitTurn_WAITTURN(0x1f));
	
	//delay time
 	TIM_Waitms(10);

  	return;
}

/*********************************************************************//**
 * @brief 		Toggle Bit check if the data is written or erased
 * @param[in]	Addr	address value
 * @return 		Checking result, could be:
 * 					- TRUE: Done
 *					- FALSE: Timeout
 **********************************************************************/
uint32_t Check_Toggle_Ready (uint32_t Addr)
{
    uint16_t PreData;
    uint16_t CurrData;
    unsigned long TimeOut = 0;

    PreData = sys_ADDR(Addr);
    PreData = PreData & 0x0040;          // read DQ6
    while (TimeOut < MAX_TIMEOUT)	 // MAX_TIMEOUT=0x07FFFFFF
    {
        CurrData = sys_ADDR(Addr);
        CurrData = CurrData & 0x0040;    // read DQ6 again
        if (PreData == CurrData)
        {
	    return TRUE;
        }
	PreData = CurrData;
        TimeOut++;
    }
    return FALSE;
}
uint32_t Check_Toggle_Ready_Abs (uint32_t Addr_Abs)
{
    uint16_t PreData;
    uint16_t CurrData;
    unsigned long TimeOut = 0;

    PreData = *(volatile uint16_t *)Addr_Abs;
    PreData = PreData & 0x0040;          // read DQ6
    while (TimeOut < MAX_TIMEOUT)	 // MAX_TIMEOUT=0x07FFFFFF
    {
        CurrData = *(volatile uint16_t *)Addr_Abs;
        CurrData = CurrData & 0x0040;    // read DQ6 again
        if (PreData == CurrData)
        {
	    return TRUE;
        }
	PreData = CurrData;
        TimeOut++;
    }
    return FALSE;
}
/*********************************************************************//**
 * @brief 		Check ID from external NOR FLASH memory
 * @param[in]	None
 * @return 		Checking result, could be:
 * 					- TRUE: Correct
 *					- FALSE: Incorrect
 **********************************************************************/
uint32_t NORFLASHCheckID( void )
{
    volatile uint16_t SST_id1, SST_id2;
    uint32_t Return_Status = FALSE; 
    sys_ADDR(0x555) = 0x00AA;   // write data 0x00AA to device addr 0x555
    sys_ADDR(0x2AA) = 0x0055;   // write data 0x0055 to device addr 0x2AA
    sys_ADDR(0x555) = 0x0090;   // write data 0x0090 to device addr 0x555
  
    delay(100);
    // Read the product ID
    SST_id1 = sys_ADDR(0x0000);
    SST_id2 = sys_ADDR(0x0001);
    
    delay(10);

    /* Check ID */
    if ((SST_id1 == SST_ID)&&(SST_id2 ==SST_39VF6401B)||((SST_id1 == SST_ID)\
        && (SST_id2 == SST_39VF6402B)))
    {
         Return_Status = TRUE;     //There is a SST39VF640X device on the EMC
    }
    else
    {
        Return_Status = FALSE;   
    }
    //==================================================
    //Software ID Exit,Return to normal operation
    sys_ADDR(0x555) = 0x00AA;   // write data 0x00AA to device addr 0x555
    sys_ADDR(0x2AA) = 0x0055;   // write data 0x0055 to device addr 0x2AA
    sys_ADDR(0x555) = 0x00F0;   // write data 0x00F0 to device addr 0x555
    delay(300);
	return( Return_Status );
}

void NORFLASHEraseSection_Abs( uint32_t Addr )
{
    sys_ADDR(0x555) = 0x00AA;   // write data 0x00AA to device addr 0x555
    sys_ADDR(0x2AA) = 0x0055;   // write data 0x0055 to device addr 0x2AA
    sys_ADDR(0x555) = 0x0080;   // write data 0x0080 to device addr 0x555    
    sys_ADDR(0x555) = 0x00AA;   // write data 0x00AA to device addr 0x555
    sys_ADDR(0x2AA) = 0x0055;   // write data 0x0055 to device addr 0x2AA
    *(volatile uint16_t *)Addr = 0x0050;   // write data 0x0010 to device addr 0x555
    
    TIM_Waitms(26);				
    return;

}
/*********************************************************************//**
 * @brief 		Erase external NOR FLASH memory
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void NORFLASHErase( void )
{
    sys_ADDR(0x555) = 0x00AA;   // write data 0x00AA to device addr 0x555
    sys_ADDR(0x2AA) = 0x0055;   // write data 0x0055 to device addr 0x2AA
    sys_ADDR(0x555) = 0x0080;   // write data 0x0080 to device addr 0x555    
    sys_ADDR(0x555) = 0x00AA;   // write data 0x00AA to device addr 0x555
    sys_ADDR(0x2AA) = 0x0055;   // write data 0x0055 to device addr 0x2AA
    sys_ADDR(0x555) = 0x0010;   // write data 0x0010 to device addr 0x555
    
    delay(10000000);				/* Use timer 1 */
    return;

}

/*********************************************************************//**
 * @brief 		Program one 16-bit data into external NOR FLASH memory
 *				This "uint16_t" for the external flash is 16 bits!!!
 * @param[in]	Addr	Address value
 * @param[in]	Data	data value
 * @return 		Program result, could be:
 					- TRUE: succesful
					- FALSE: fail
 **********************************************************************/
uint32_t NORFLASHWriteWord( uint32_t Addr, uint16_t Data )
{
    sys_ADDR(0x555) = 0x00AA;   // write data 0x00AA to device addr 0x555  
    sys_ADDR(0x2AA) = 0x0055;   // write data 0x0055 to device addr 0x2AA
    sys_ADDR(0x555) = 0x00A0;   // write data 0x00AA to device addr 0x555
    
    sys_ADDR(Addr) = Data;      // Program 16-bit word 
    
    return ( Check_Toggle_Ready(Addr) );
}
uint32_t NORFLASHWriteWord_Abs( uint32_t Addr, uint16_t Data )
{
    sys_ADDR(0x555) = 0x00AA;   // write data 0x00AA to device addr 0x555  
    sys_ADDR(0x2AA) = 0x0055;   // write data 0x0055 to device addr 0x2AA
    sys_ADDR(0x555) = 0x00A0;   // write data 0x00AA to device addr 0x555
    
    *(volatile uint16_t *)Addr = Data;      // Program 16-bit word 
    
    return ( Check_Toggle_Ready_Abs(Addr) );
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
