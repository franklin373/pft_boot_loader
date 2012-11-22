#include "global.h"
#include "flash_common.h"


FLASH_COMMON_INTERFACE flash_common_intf_array[2];
int flash_common_intf_array_len;

int flash_intf_init()
{
	flash_common_flash1788_init(&flash_common_intf_array[0]);
	flash_common_flash_sst39vf6401b_init(&flash_common_intf_array[1]);
	flash_common_intf_array_len=2;
	return 0;
}
#define ADDRESS_IN_SECT(addr,sect) (sectorStartAddrArray[sect]<=(addr)&&(addr)<sectorStartAddrArray[sect+1])
int flash_erase_one(FLASH_COMMON_INTERFACE *pIntf,uint32 addr,int size)
{
	int sect,sectBegin,sectEnd;
	uint32 *sectorStartAddrArray=pIntf->fci_get_sector_address();

	_DBG("Enter flash_erase_one,pIntf=");
	_DBH32((uint32)pIntf);
	_DBG(",");
	_DBD32(pIntf->fci_get_program_unit_size());
	_DBG("\r\n");
	sectBegin=-1;
	sectEnd=-1;
	for(sect=0;sect<sectorStartAddrArray[-1];++sect){
#if 0		
if(pIntf->fci_get_program_unit_size()==256){	
	_DBG("<sect,addr,sectorStartAddrArray[sect]>");
	_DBD32(sect);
	_DBG(",");
	_DBH32(addr);
	_DBG(",");
	_DBH32(sectorStartAddrArray[sect]);
	_DBG("\r\n");
}	
#endif
		if(ADDRESS_IN_SECT(addr,sect)){
			sectBegin=sect;
		}
		if(ADDRESS_IN_SECT(addr+size-1,sect)){
			sectEnd=sect;
		}
		if(sectBegin>=0&&sectEnd>=0){
			goto SUCC;
		}
	}
	return -1;
SUCC:
	_DBG("<sectBegin,sectEnd>");
	_DBH32(sectBegin);
	_DBG(",");
	_DBH32(sectEnd);
	_DBG("\r\n");
	for(sect=sectBegin;sect<=sectEnd;++sect){
		pIntf->fci_erase_sector(sect);
	}
	return 0;
}
int flash_erase(uint32 addr,int size)
{
	int i;
	int nRtn;

	for(i=0;i<flash_common_intf_array_len;i++){
		nRtn=flash_erase_one(&flash_common_intf_array[i],addr,size);
		if(nRtn==0){
			return 0;
		}
	}
	return -1;
}
static uint32 buf_for_bufbuf[256/4];
static  uint8 *bufbuf=(uint8 *)buf_for_bufbuf;
int flash_program_once(FLASH_COMMON_INTERFACE *pIntf,uint32 dest,uint8 *src,int len)
{
	int unit_size;
	uint32 unit_begin;
#if 0
	_DBG("flash_program_once<dest,len,src[0]>");
	_DBH32(dest);
	_DBG(",");
	_DBD32(len);
	_DBG(",");
	_DBH(src[0]);
	_DBG("\n\r");
#endif

	unit_size=pIntf->fci_get_program_unit_size();
	unit_begin=(dest&~(unit_size-1));
	ASSERT_MINE(unit_begin+unit_size>=dest+len);
	if(unit_size>len){
		memcpy(bufbuf,(char *)unit_begin,unit_size);
	}
	memcpy(bufbuf+(dest-unit_begin),src,len);
#if 0
	_DBG("flash_program_once<unit_begin,unit_size,bufbuf[0]>");
	_DBH32(unit_begin);
	_DBG(",");
	_DBD32(unit_size);
	_DBG(",");
	_DBH(bufbuf[0]);
	_DBG("\n\r");
#endif
	ASSERT_MINE(0==pIntf->fci_program(unit_begin,bufbuf));
	return 0;
}
int flash_valid_region(FLASH_COMMON_INTERFACE *pIntf,uint32 begin,uint32 end)
{
	uint32 *sectorStartAddrArray;
	uint32 chipStart,chipEnd;

	sectorStartAddrArray=pIntf->fci_get_sector_address();
	chipStart=sectorStartAddrArray[0];
	chipEnd=sectorStartAddrArray[sectorStartAddrArray[-1]];
	if(chipStart<=begin&&end<=chipEnd)
		return 1;
	else
		return 0;
}
int flash_program_device(FLASH_COMMON_INTERFACE *pIntf,uint32 dest,uint8 *src,int len)
{
	int unit_size;
	uint32 unitStart,unitEnd;
	uint32 intersectStart,intersectEnd;
	int i;
	
	if(!flash_valid_region(pIntf,dest,dest+len)){
		return -1;
	}
	unit_size=pIntf->fci_get_program_unit_size();
	for(i=0;;i++){
		unitStart=(dest&~(unit_size-1))+i*unit_size;
		unitEnd=(dest&~(unit_size-1))+(i+1)*unit_size;
		intersectStart=MAX(unitStart,dest);
		intersectEnd=MIN(unitEnd,dest+len);
		if(intersectStart>=intersectEnd){
			break;
		}
		ASSERT_MINE(0==flash_program_once(pIntf,intersectStart,src+(intersectStart-dest),intersectEnd-intersectStart));
	}
	return 0;
}
int flash_program(uint32 addr,uint8 *buf,int len)
{
	int i;
	int nRtn;

	_DBG("flash_program");
	_DBH32(addr);
	_DBG(",");
	_DBD32(len);
	_DBG("\n\r");
	for(i=0;i<flash_common_intf_array_len;i++){
		nRtn=flash_program_device(&flash_common_intf_array[i],addr,buf,len);
		if(nRtn==0){
			return 0;
		}
	}
	return -1;
}
void flash_disp_for_debug()
{
	int i,j;
	FLASH_COMMON_INTERFACE *pIntf;

	for(i=0;i<flash_common_intf_array_len;i++){
		pIntf=&flash_common_intf_array[i];
		uint32 *sectorStartAddrArray=pIntf->fci_get_sector_address();

		_DBG("flash device");
		_DBD32(i);
		_DBG(",");
		_DBH32((uint32)pIntf);
		_DBG("----");
		for(j=0;j<5;j++){
			_DBG(",");
			_DBH32(sectorStartAddrArray[j]);
		}
		_DBG("\r\n");
	}
}
