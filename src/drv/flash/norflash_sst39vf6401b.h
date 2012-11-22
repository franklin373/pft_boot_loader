/***********************************************************************//**
 * @file		norflash_sst39vf6401b.h
 * @brief		Contains all macro definitions and function prototypes
 * 				support for external NOR Flash SST39VF6401B
 * @version		1.0
 * @date		2011.8.5
 * @author		Tenghua SW Application Team
 **************************************************************************
 **************************************************************************/

#ifndef NORFLASH_SST39VF6401B_H_
#define NORFLASH_SST39VF6401B_H_

#include "lpc177x_8x_emc.h"

/*****************************************************************************
 * Defines and typedefs
 ****************************************************************************/
#define NOR_FLASH_BASE		0x80000000	    //CS0 start address
#define NOR_FLASH_SIZE		0x00400000      //4M*16bit

#define sys_ADDR(offset)  *((volatile uint16_t *)(NOR_FLASH_BASE+(offset<<1)))



#define GET_ADDR(addr)	(volatile uint16_t *)(NOR_FLASH_BASE | addr)

#define SECTOR_SIZE		    0x800	/* Must be 2048 words */
#define BLOCK_SIZE		    0x8000	/* Must be 32K words  */

#define SST_ID			    0xBF    /* SST Manufacturer's ID code   */
#define SST_39VF1601		0x234B  /* SST 39VF160 device code      */
#define SST_39VF3201		0x235B  /* SST 39VF160 device code      */
#define SST_39VF6401		0x236B  /* SST 39VF6401 device code     */
#define SST_39VF6401B       0x236D  /* SST 39VF6401B device code    */
#define SST_39VF6402B       0x236C  /* SST 39VF6401B device code    */


#define PROGRAM_TIMEOUT	0x08000000
#define MAX_TIMEOUT	0x07FFFFFF   // A ceiling constant used by Check_Toggle_
	                             // Ready() and Check_Data_Polling().
      	                        // The user should modify this constant accordingly.

extern void NORFLASHInit( void );
extern void NORFLASHErase( void );
extern uint32_t NORFLASHCheckID( void );
extern uint32_t NORFLASHWriteWord( uint32_t Addr, uint16_t Data );
extern uint32_t Check_Toggle_Ready (uint32_t Addr);
void NORFLASHEraseSection_Abs( uint32_t Addr );
uint32_t NORFLASHWriteWord_Abs( uint32_t Addr, uint16_t Data );

#endif /* NORFLASH_SST39VF6401B_H_ */
