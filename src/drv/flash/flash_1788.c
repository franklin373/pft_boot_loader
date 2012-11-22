#include "global.h"
#include "flash_common.h"
#include "lpc177x_8x_timer.h"
#include "IAP.h"
/*
typedef int *(*FCI_FUNC_GET_SECTOR_ADDRESS)(FLASH_COMMON_INTERFACE *pIntf);
typedef int (*FCI_FUNC_GET_PROGRAM_UNIT_SIZE)(FLASH_COMMON_INTERFACE *pIntf);
typedef int (*FCI_FUNC_ERASE_SECTOR)(FLASH_COMMON_INTERFACE *pIntf,int sectorNumber);
typedef int (*FCI_FUNC_PROGRAM)(FLASH_COMMON_INTERFACE *pIntf,uint32 addrDest,uint32 addrSrc);
*/
#define FLASH1788_UNIT_PROG_LEN (256)
static uint32 flash1788_section_address[]={
30,/*30 section, flash1788_section_address would need 32, first section no, last limit*/
0x0,
0x1000,
0x2000,
0x3000,
0x4000,
0x5000,
0x6000,
0x7000,
0x8000,
0x9000,
0xA000,
0xB000,
0xC000,
0xD000,
0xE000,
0xF000,
0x10000,
0x18000,
0x20000,
0x28000,
0x30000,
0x38000,
0x40000,
0x48000,
0x50000,
0x58000,
0x60000,
0x68000,
0x70000,
0x78000,
0x80000,
};
uint32 *flash1788_get_sector_address()
{
	return &flash1788_section_address[1];
}
int flash1788_get_program_unit_size()
{
	return FLASH1788_UNIT_PROG_LEN;
}
int flash1788_erase_sector(int sectorNumber)
{
	if(sectorNumber<0||sectorNumber>=flash1788_section_address[0]){
		ASSERT_MINE(0);
	}
	ASSERT_MINE(u32IAP_PrepareSectors(sectorNumber, sectorNumber) == IAP_STA_CMD_SUCCESS);
	ASSERT_MINE(u32IAP_EraseSectors  (sectorNumber, sectorNumber) == IAP_STA_CMD_SUCCESS);
	return 0;
}
int flash1788_program(uint32 addrDest,uint8 *addrSrc)
{
	int i;
	int sectorNumber;
	
	for(i=0;;i++){
		if(i>=flash1788_section_address[0]){
			return -1;
		}
		if(flash1788_section_address[i+1]<=addrDest&&addrDest<flash1788_section_address[i+2]){
			break;
		}
	}
	sectorNumber=i;
	ASSERT_MINE((addrDest&(FLASH1788_UNIT_PROG_LEN-1))==0);
	ASSERT_MINE(u32IAP_PrepareSectors(sectorNumber, sectorNumber) == IAP_STA_CMD_SUCCESS);
	ASSERT_MINE(u32IAP_CopyRAMToFlash(addrDest, (uint32_t)addrSrc, FLASH1788_UNIT_PROG_LEN) == IAP_STA_CMD_SUCCESS);
	return 0;
}
void flash_common_flash1788_init(FLASH_COMMON_INTERFACE *pIntf)
{
	pIntf->fci_get_sector_address=flash1788_get_sector_address;
	pIntf->fci_get_program_unit_size=flash1788_get_program_unit_size;
	pIntf->fci_erase_sector=flash1788_erase_sector;
	pIntf->fci_program=flash1788_program;
}
