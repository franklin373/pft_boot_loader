#include "global.h"
#include "flash_common.h"
#include "norflash_sst39vf6401b.h"
/*
typedef int *(*FCI_FUNC_GET_SECTOR_ADDRESS)(FLASH_COMMON_INTERFACE *pIntf);
typedef int (*FCI_FUNC_GET_PROGRAM_UNIT_SIZE)(FLASH_COMMON_INTERFACE *pIntf);
typedef int (*FCI_FUNC_ERASE_SECTOR)(FLASH_COMMON_INTERFACE *pIntf,int sectorNumber);
typedef int (*FCI_FUNC_PROGRAM)(FLASH_COMMON_INTERFACE *pIntf,uint32 addrDest,uint32 addrSrc);
*/
//#define PER_SECTION_SIZE (4*1024)
//#define TOTAL_SECTION_NUM (2048)
static uint32 flash_sst39vf6401b_section_address[NOR_FLASH_SIZE/SECTOR_SIZE+2];
void init_flash_sst39vf6401b_section_address()
{
	int section;

	flash_sst39vf6401b_section_address[0]=NOR_FLASH_SIZE/SECTOR_SIZE;
	for(section=0;section<=NOR_FLASH_SIZE/SECTOR_SIZE;section++){
		flash_sst39vf6401b_section_address[section+1]=NOR_FLASH_BASE+section*SECTOR_SIZE*2;
	}
}
uint32 *flash_sst39vf6401b_get_sector_address()
{
	return &flash_sst39vf6401b_section_address[1];
}
int flash_sst39vf6401b_get_program_unit_size()
{
	return 2;
}
int flash_sst39vf6401b_erase_sector(int sectorNumber)
{
	ASSERT_MINE(sectorNumber>=0);
	ASSERT_MINE(sectorNumber<flash_sst39vf6401b_section_address[0]);
	NORFLASHEraseSection_Abs(flash_sst39vf6401b_section_address[sectorNumber+1]);
	return 0;
}
int flash_sst39vf6401b_program(uint32 addrDest,uint8 *addrSrc)
{
	uint16 addrData;

	memcpy(&addrData,addrSrc,2);
	NORFLASHWriteWord_Abs(addrDest,addrData);
	return 0;
}
void flash_common_flash_sst39vf6401b_init(FLASH_COMMON_INTERFACE *pIntf)
{
	init_flash_sst39vf6401b_section_address();
	pIntf->fci_get_sector_address=flash_sst39vf6401b_get_sector_address;
	pIntf->fci_get_program_unit_size=flash_sst39vf6401b_get_program_unit_size;
	pIntf->fci_erase_sector=flash_sst39vf6401b_erase_sector;
	pIntf->fci_program=flash_sst39vf6401b_program;
}